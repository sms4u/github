/*******************************************************************************
 COPYRIGHT NOTICE:
    Core Banker TM
    Copyright (c) Footprint Software Inc. 1995 - 1996
    All Rights Reserved
    Licensed Material - Property of Footprint Software
*******************************************************************************/
/* apslapi.cpp */

#include "apslapi.h"
#include "CFSHA.h"
#include "abpresn.hpp"
#include "abhannod.hpp"
#include "abhanmsg.hpp"

#ifdef __WINDOWS__
#define asCDATE asICnrDate
#define asCTIME asICnrTime
#endif

static CcabPresentationObject*
   CgPO = NULL;

static CcabListParmCon*
   DiagnosticList = NULL;

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_CfSHA(
   CtChar* pInput,
   CtVoid* pValue)
////////////////////////////////////////////////////////////////////////////////
{
char output[41];
   CfSHA(pInput, output);
   strncpy((CtChar*)pValue, output, 40);
   return(0);
}

////////////////////////////////////////////////////////////////////////////////
void CfUINodeIterator(
   const CcabCollaborationNodeHandle& pNodeHandle,
   CcabObject* pThisP)
////////////////////////////////////////////////////////////////////////////////
{
nodeIterator(pNodeHandle);
};

////////////////////////////////////////////////////////////////////////////////
void nodeIterator(
   const CcabCollaborationNodeHandle& pNodeHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtString string;
CcabCOID coid;
CtInteger rc;

coid = CgPO->ownerCOID(pNodeHandle);
string = coid.platformID()+":"+coid.classID()+":"+coid.uniqueID();
rc = (CtInteger)(CgPO->getNodeResult(pNodeHandle));
if (rc != 0)
   {
   CcabParameterContainerHandle p;
   p = CgPO->getDiagnosticParameters(pNodeHandle);
   CgPO->addParameterItem(p,"pCOID",string);
   DiagnosticList->add(p);
   }
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_isEOD(
   CtBoolean* pBool)
////////////////////////////////////////////////////////////////////////////////
{
CtDate date;
CtShort rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   *pBool = CgPO->isEOD();
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_currentPostDate(
   _CDATE* pDate)
////////////////////////////////////////////////////////////////////////////////
{
CtDate date;
CtShort rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CgPO->currentPostDate(date);
   *pDate = date.asCDATE();
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_allDiagnostics(
   CcabListParmCon* pValue)
////////////////////////////////////////////////////////////////////////////////
{
CcabCOID coid;
CcabCollaborationNodeHandle cnH;
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   DiagnosticList = pValue;
   CgPO->surfacedNodesDo(&CfUINodeIterator, CgPO);
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_convertZonedDateTime(
   CtVoid* zonedDateTime1,
   CtVoid* zonedDateTime2)
////////////////////////////////////////////////////////////////////////////////
{
CcbbZonedDateTime z1, z2;
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   _CDATE* d = (_CDATE*)zonedDateTime1;
   _CTIME* t = (_CTIME*)(d+1);
   z1 = CcbbZonedDateTime
        (CtDate(*d),
        CtTime(*t),
        CcbbTranslatableString(*((CtInteger*)(t+1))));

   CtInteger mri = *((CtInteger*)(((long*)zonedDateTime2)+2));
   const CcbbTimeZone*
      pZone = CcbbTimeZone::find(CcbbTranslatableString(mri));
   z2 = z1.toZone(*pZone);
   *((_CDATE*)zonedDateTime2) = z2.getDate().asCDATE();
   *((_CTIME*)((_CDATE*)zonedDateTime2+1)) = z2.getTime().asCTIME();
   }
return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_begin(
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO != NULL)
   {
   rc = APSL_ALREADY_LOADED;
   }
else
   {
   CcabCOID coid(pPlatformID, pClassID, pUniqueID);

   CgPO = new CcabPresentationObject(coid, 'U');
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_end()
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete CgPO;
   CgPO = NULL;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_setTimeout(
   CtInteger pTimeout)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   rc = APSL_NOT_LOADED;
else
   CgPO->setTimeout(pTimeout);

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_responseMethodName(CtChar* pName)
////////////////////////////////////////////////////////////////////////////////
{
CtShort rc;
CtString str;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   str = CgPO->responseMethodName();
   #ifdef INC_DUMP
   ofstream fout("e:\\ap.out",append);
   fout << "[ResponseName] " << str << endl;
   fout.close();
   #endif
   strcpy(pName,(CtChar*)str);
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteNodeHandle(
   CcabCollaborationNodeHandle* pNodeHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pNodeHandle;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteParameterContainerHandle(
   CcabParameterContainerHandle* pParmHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pParmHandle;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteListParmCon(
   CcabListParmCon* pListParmCon)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pListParmCon;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteListInteger(
   CcabListInteger* pListInteger)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pListInteger;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteListString(
   CcabListString* pListString)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pListString;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteTargetQueueHandle(
   CcabTargetQueueHandle* pTargetQueueHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pTargetQueueHandle;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_deleteNodeContainerHandle(
   CcabNodeContainerHandle* pNodeContainerHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   delete pNodeContainerHandle;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_surfacedNodesCopy(
   CcabNodeContainerHandle** pNodeContainerHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabNodeContainerHandle* p = new CcabNodeContainerHandle();

   *p = CgPO->surfacedNodesCopy();

   *pNodeContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_newTargetQueue(
   CcabTargetQueueHandle** pTargetQueueHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabTargetQueueHandle* p = new CcabTargetQueueHandle();

   *p = CgPO->newTargetQueue();

   *pTargetQueueHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_addTarget(
   CcabTargetQueueHandle* pTargetQueueHandle,
   CtChar* pMethodID,
   CtChar* pClassID,
   CcabParameterContainerHandle* pHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   if (pTargetQueueHandle)
      CgPO->addTarget(*pTargetQueueHandle,pMethodID,pClassID,*pHandle);
   else
      rc = APSL_ERROR;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_newParameters(
   CcabParameterContainerHandle** pParmContainerHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabParameterContainerHandle* p = new CcabParameterContainerHandle();

   *p = CgPO->newParameters();

   *pParmContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_newNodeContainer(
   CcabNodeContainerHandle** pNodeContainerHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabNodeContainerHandle* p = new CcabNodeContainerHandle();

   *p = CgPO->newNodeContainer();

   *pNodeContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_currentParameters(
   CcabParameterContainerHandle** pParmContainerHandle,
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabCollaborationNodeHandle cnH;
   CcabParameterContainerHandle* p = new CcabParameterContainerHandle();

   if (pPlatformID == NULL || pClassID == NULL || pUniqueID == NULL)
      {
      coid = CgPO->senderCOID();
      }
   else
      {
      coid.platformID(pPlatformID);
      coid.classID(pClassID);
      coid.uniqueID(pUniqueID);
      }

   if (CgPO->nodeAt(CgPO->surfacedNodes(),coid,cnH) == CC_NO_ERROR)
      *p = CgPO->getMessageParameters(cnH);
   else
      *p = CcabParameterContainerHandle(
         CE_CURRENT_MESSAGE,
         CE_MESSAGE_PARAMETERS);

   *pParmContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_overallResult(
   CtInteger* pResult)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   rc = APSL_NOT_LOADED;
else
   *pResult = (CtInteger)(CgPO->overallResult());

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_getNodeResult(
   CtInteger* pResult,
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabCollaborationNodeHandle cnH;

   if (pPlatformID == NULL || pClassID == NULL || pUniqueID == NULL)
      {
      coid = CgPO->senderCOID();
      }
   else
      {
      coid.platformID(pPlatformID);
      coid.classID(pClassID);
      coid.uniqueID(pUniqueID);
      }

   if (CgPO->nodeAt(CgPO->surfacedNodes(),coid,cnH) == CC_NO_ERROR)
      *pResult = (CtInteger)(CgPO->getNodeResult(cnH));

   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_diagnosticParameters(
   CcabParameterContainerHandle** pParmContainerHandle,
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabCollaborationNodeHandle cnH;
   CcabParameterContainerHandle* p = new CcabParameterContainerHandle();

   if (pPlatformID == NULL || pClassID == NULL || pUniqueID == NULL)
      {
      coid = CgPO->senderCOID();
      }
   else
      {
      coid.platformID(pPlatformID);
      coid.classID(pClassID);
      coid.uniqueID(pUniqueID);
      }

   if (CgPO->nodeAt(CgPO->surfacedNodes(),coid,cnH) == CC_NO_ERROR)
      *p = CgPO->getDiagnosticParameters(cnH);

   *pParmContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_contextParameters(
   CcabParameterContainerHandle** pParmContainerHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
CcabCOID
   coid;
const CcabCoarseMessageHandle
   CC_CURRENT_MESSAGE(CE_CURRENT_MESSAGE);

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabParameterContainerHandle* p = new CcabParameterContainerHandle();

   *p = CgPO->getContextParameters(CC_CURRENT_MESSAGE);

   *pParmContainerHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_copyNodeForContext(
   CcabCollaborationNodeHandle** pNodeHandle,
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc, rc1;
CcabCOID
   coid;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabCollaborationNodeHandle* p = new CcabCollaborationNodeHandle();

   if (pPlatformID == NULL || pClassID == NULL || pUniqueID == NULL)
      {
      coid = CgPO->senderCOID();
      }
   else
      {
      coid.platformID(pPlatformID);
      coid.classID(pClassID);
      coid.uniqueID(pUniqueID);
      }

   rc1 = CgPO->copyNodeForContext(CgPO->surfacedNodes(),coid,*p);
   if (rc1 != CC_NO_ERROR)
      {
      rc = APSL_COPY_NODE_FOR_CONTEXT_FAILED;
      delete p;
      p = NULL;
      }

   *pNodeHandle = p;
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_setContextParameters(
   CcabNodeContainerHandle* pNodeContainerHandle,
   CcabParameterContainerHandle* pHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort rc;
CcabCollaborationNodeHandle* pNodeH;

if (CgPO->nodeAt(*pNodeContainerHandle,CgPO->targetCOID(),*pNodeH) == CC_NOT_FOUND)
   {
   CcabCollaborationNode node;
   node.coid(CgPO->targetCOID());
   CgPO->addNode(*pNodeContainerHandle,CcabCollaborationNodeHandle(&node));
   }
rc = CgPO->setContextParameters(*pNodeContainerHandle,*pHandle);
return(rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_addNode(
   CcabNodeContainerHandle* pNodeContainerHandle,
   CcabCollaborationNodeHandle* pNodeHandle)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   if (CgPO->addNode(*pNodeContainerHandle,*pNodeHandle) != CC_NO_ERROR)
      rc = APSL_ADD_NODE_FAILED;
   }

return (rc);
}

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_addParameterItem(
   CcabParameterContainerHandle* pParmHandle,
   CtChar* pName,
   CtVoid* pValue,
   CtChar pType)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   #ifdef INC_DUMP
   ofstream fout("e:\\ap.out",append);
   fout << "[ADD] " << pType << ":" << pName << ";" << pValue << endl;
   fout.close();
   #endif
   switch (pType)
      {
      case 'L':
         {
         CtDecimal
            item = *((CtDecimal*) pValue);

         CgPO->addParameterItem(*pParmHandle,pName,CtString(item));

         break;
         }
      case 'I':
         {
         CtInteger
            item = *((CtInteger*) pValue);

         CgPO->addParameterItem(*pParmHandle,pName,item);

         break;
         }
      case 'S':
         {
         CgPO->addParameterItem(
            *pParmHandle,
            pName,
            (CtChar*) pValue
            );
         break;
         }
      case 'Z':
         {
         CcbbZonedDateTime zonedDateTime;

         _CDATE* d = (_CDATE*)pValue;
         _CTIME* p = (_CTIME*)(d+1);

         switch (*((CtInteger*)d))
         {
         case 0:
            {
            zonedDateTime = CM_INVALID_ZONED_DATETIME;
            break;
            }
         case -1:
            {
            zonedDateTime = CM_MIN_ZONED_DATETIME;
            break;
            }
         case 1:
            {
            zonedDateTime = CM_MAX_ZONED_DATETIME;
            break;
            }
         default:
            {
            zonedDateTime = CcbbZonedDateTime
                            (CtDate(*d),
                            CtTime(*p),
                            CcbbTranslatableString(*((CtInteger*)(p+1))));
            break;
            }
         }
         CgPO->addParameterItem(*pParmHandle,pName,zonedDateTime.asParameter());

         break;
         }
      case 'D':
         {
         CtDate item = CtDate(1,CtDate::January,1);
         _CDATE* d = (_CDATE*)pValue;

         if (*((long*)d) != 0)
            item = CtDate(*d);

         CgPO->addParameterItem(*pParmHandle,pName,item);

         break;
         }
      case 'T':
         {
         CtTime item = CtTime(0);
         _CTIME* t = (_CTIME*)pValue;

         if (*((long*)t) != 0)
            item = CtTime(*t);

         CgPO->addParameterItem(*pParmHandle,pName,item);

         break;
         }
      case 'O':
         {
         CtDecimal* pAmount = (double*)pValue;
         CtInteger* pCurrency = (CtInteger*)((double*)pValue + 1);
         CcbbMonetaryValue m = CcbbMonetaryValue(*pAmount,(CeCurrency)*pCurrency);

         CgPO->addParameterItem(*pParmHandle,pName,m.asParameter());

         break;
         }
      case 'R':
         {
         CcbbRecurringInterval rInterval;

         if (*((long*)pValue) == 0)
            rInterval = CM_INVALID_RECURRING_INTERVAL;
         else
            {
            _CDATE* pFrom = (_CDATE*)pValue;
            _CDATE* pTo = (_CDATE*)pValue + 2;

            CcbbZonedDateTime fromZ = CM_MIN_ZONED_DATETIME;
            CcbbZonedDateTime toZ = CM_MAX_ZONED_DATETIME;
            if (*((CtInteger*)pFrom) != 0)
            {
              if (*((CtInteger*)pFrom+1) != 0)
                fromZ = CcbbZonedDateTime(CtDate(*pFrom),CtTime(*((_CTIME*)pFrom+1)));
              else
                fromZ = CcbbZonedDateTime(CtDate(*pFrom));
            }
            if (*((CtInteger*)pTo) != 0)
            {
              if (*((CtInteger*)pTo+1) != 0)
                toZ = CcbbZonedDateTime(CtDate(*pTo),CtTime(*((_CTIME*)pTo+1)));
              else
                toZ = CcbbZonedDateTime(CtDate(*pTo));
            }
            CcbbInterval delimit(fromZ, toZ, false);

            pFrom = (_CDATE*)pValue+4;
            pTo = (_CDATE*)pValue+6;

            if (*((CtInteger*)pFrom+1) != 0)
              fromZ = CcbbZonedDateTime(CtDate(*pFrom),CtTime(*((_CTIME*)pFrom+1)));
            else
              fromZ = CcbbZonedDateTime(CtDate(*pFrom));

            if (*((CtInteger*)pTo+1) != 0)
              toZ = CcbbZonedDateTime(CtDate(*pTo),CtTime(*((_CTIME*)pTo+1)));
            else
              toZ = CcbbZonedDateTime(CtDate(*pTo));
            CcbbInterval seed(fromZ, toZ, false);

            CtShort* p = (CtShort *)((_CDATE*)pValue+8);
            CcbbDuration
               period(*p,
                      *(p+1),
                      *(p+2),
                      *(p+3));

            rInterval = CcbbRecurringInterval(delimit, seed, &period);
            }

         CgPO->addParameterItem(*pParmHandle,pName,rInterval.asParameter());

         break;
         }
      case 'V':
         {
         _CDATE* pFrom = (_CDATE*)pValue;
         _CDATE* pTo = (_CDATE*)pValue+2;

         CcbbZonedDateTime fromZ = CM_MIN_ZONED_DATETIME;
         CcbbZonedDateTime toZ = CM_MAX_ZONED_DATETIME;
         if (*((CtInteger*)pFrom) != 0)
           fromZ = CcbbZonedDateTime(CtDate(*pFrom),CtTime(*((_CTIME*)pFrom+1)));

         if (*((CtInteger*)pTo) != 0)
           toZ = CcbbZonedDateTime(CtDate(*pTo),CtTime(*((_CTIME*)pTo+1)));

         CcbbInterval interval(fromZ, toZ, false);

         CgPO->addParameterItem(*pParmHandle,pName,interval.asParameter());

         break;
         }
      #ifdef INC_BB
      case 'M':
         {
         CcbbTranslatableString
            item(*((CtMri*) pValue));

         CgPO->addParameterItem(*pParmHandle,pName,item.asParameter());

         break;
         }
      #endif
      case 'C':
         {
         CtChar
            record[APSL_REC_SZ];
         CtChar
            *platformID,
            *classID,
            *uniqueID;

         strcpy(record,(CtChar*) pValue);

         platformID = strtok(record,":");
         classID    = strtok(NULL,":");
         uniqueID   = strtok(NULL,":");

         CgPO->addParameterItem(
            *pParmHandle,
            pName,
            CcabCOID(platformID,classID,uniqueID).asParameter());

         break;
         }
      case 'B':
         {
         CtBoolean
            item = *((CtBoolean*) pValue);

         CgPO->addParameterItem(*pParmHandle,pName,item);

         break;
         }
      case 'U':
         {
         CcbbDuration
            duration(*((CtShort*)pValue),
                     *((CtShort*)pValue+1),
                     *((CtShort*)pValue+2),
                     *((CtShort*)pValue+3));

         CgPO->addParameterItem(*pParmHandle,pName,duration.asParameter());

         break;
         }
      case 'd':
         {
         CgPO->addParameterItem(*pParmHandle,pName,((CcabListDate*)pValue)->asParameter());
         break;
         }
      case 'i':
         {
         CgPO->addParameterItem(*pParmHandle,pName,((CcabListInteger*)pValue)->asParameter());
         break;
         }
      case 's':
         {
         CgPO->addParameterItem(*pParmHandle,pName,((CcabListString*)pValue)->asParameter());
         break;
         }
      case 'r':
         {
         CgPO->addParameterItem(*pParmHandle,pName,*((CcabParameterContainerHandle*)pValue));
         break;
         }
      case 'p':
         {
         CgPO->addParameterItem(*pParmHandle,pName,((CcabListParmCon*)pValue)->asParameter());
   #ifdef INC_DUMP
   ofstream fout("e:\\ap.out",append);
   fout << "---> " << ((CcabListParmCon*)pValue)->asParameter() << endl;
   fout.close();
   #endif
         break;
         }
      default:
         {
         rc = APSL_ERROR;
         break;
         }
      }
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_getParameterItem(
   CcabParameterContainerHandle* pParmHandle,
   CtChar* pName,
   CtVoid* pValue,
   CtChar pType,
   CtVoid* pBuffer)
////////////////////////////////////////////////////////////////////////////////
{
CcabItemState* pState = new CcabItemState();
CtShort rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   #ifdef INC_DUMP
   ofstream fout("e:\\ap.out",append);
   fout << "[GET] " << pType << ":" << pName << ":" << pParmHandle << endl;
   fout.close();
   #endif

   switch (pType)
      {
      case 'L':
         {
         CtString
            string;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         *((CtDecimal*)pValue)=(CtDecimal)string.asDouble();

         break;
      }
      case 'I':
         {
         CtInteger
            integer;

         rc = CgPO->getParameterItem(*pParmHandle,pName,integer,pState);

         *((CtInteger*) pValue) = integer;

         break;
         }
      case 'i':
         {
         CtString
            string;
         CcabListInteger*
            list = new CcabListInteger();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (rc == 0) list->fromParameter(string);

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      case 'O':
         {
         CtString string;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         CcbbMonetaryValue value;

         value.fromParameter(string);
         *((double*)pValue) = (double)value.get_amount();
         *((CtInteger*)((double*)pValue+1)) = (CtInteger)value.get_currency();

         break;
         }
      case 'S':
         {
         CtString
            string;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);
         strcpy((CtChar*)pValue,(CtChar*)string);
         break;
         }
      case 's':
         {
         CtString
            string;
         CcabListString*
            list = new CcabListString();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (rc == 0) list->fromParameter(string);

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      #ifdef INC_BB
      case 'Z':
         {
         CtString
            string;
         CcbbZonedDateTime
            zonedDateTime;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (rc != CC_NOT_FOUND)
            zonedDateTime.fromParameter(string);

         if (zonedDateTime == CM_INVALID_ZONED_DATETIME)
            *((CtInteger*)pValue) = 0;
         else
            if (zonedDateTime == CM_MIN_ZONED_DATETIME)
               *((CtInteger*)pValue) = -1;
            else
               if (zonedDateTime == CM_MAX_ZONED_DATETIME)
                  *((CtInteger*)pValue) = 1;
               else
                  {
                  CtInteger mri = *((CtInteger*)((long*)pValue+2));
                  const CcbbTimeZone*
                     pZone = CcbbTimeZone::find(CcbbTranslatableString(mri));
                  *((_CDATE*)pValue) =
                     zonedDateTime.getDate().asCDATE();
                  *((_CTIME*)((_CDATE*)pValue+1)) =
                     zonedDateTime.getTime().asCTIME();
                  *((CtInteger*)((_CDATE*)pValue+2)) =
                     zonedDateTime.zone().name().asCode();
                  }

         break;
         }
      case 'U':
         {
         CtString
            string;
         CcbbDuration
            duration;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         duration.fromParameter(string);

         *((CtShort*)pValue) =
            duration.getDays();
         *((CtShort*)pValue+1) =
            duration.getWeeks();
         *((CtShort*)pValue+2) =
            duration.getMonths();
         *((CtShort*)pValue+3) =
            duration.getYears();

         break;
         }
      case 'R':
         {
         CtString
            string;
         CcbbInterval
            seed, delimit;
         CcbbZonedDateTime zonedDateTime;
         const CcbbDuration* pDuration;
         CcbbRecurringInterval rInterval;
         CtShort* p;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         rInterval.fromParameter(string);

         delimit = rInterval.getDelimitInterval();
         seed = rInterval.getSeedInterval();
         pDuration = rInterval.getPeriod();

         if (!rInterval.isValid())
            *((double*)pValue) = 0;
         else
         if ((delimit == CM_INVALID_INTERVAL) &&
            (seed == CM_INVALID_INTERVAL) &&
            (pDuration == NULL))
            *((double*)pValue) = 0;
         else
            {
            zonedDateTime = delimit.start();
            if (zonedDateTime == CM_MIN_ZONED_DATETIME)
               *((double*)pValue) = 0;
            else
               {
               *((_CDATE*)pValue) = zonedDateTime.getDate().asCDATE();
               *((_CTIME*)pValue+1) = zonedDateTime.getTime().asCTIME();
               }

            zonedDateTime = delimit.end();
            if (zonedDateTime == CM_MAX_ZONED_DATETIME)
               *((double*)pValue+2) = 0;
            else
               {
               *((_CDATE*)pValue+2) = zonedDateTime.getDate().asCDATE();
               *((_CTIME*)pValue+3) = zonedDateTime.getTime().asCTIME();
               }

            zonedDateTime = seed.start();
            if (zonedDateTime == CM_MIN_ZONED_DATETIME)
               *((double*)pValue+4) = 0;
            else
               {
               *((_CDATE*)pValue+4) = zonedDateTime.getDate().asCDATE();
               *((_CTIME*)pValue+5) = zonedDateTime.getTime().asCTIME();
               }

            zonedDateTime = seed.end();
            if (zonedDateTime == CM_MAX_ZONED_DATETIME)
               *((double*)pValue+6) = 0;
            else
               {
               *((_CDATE*)pValue+6) = zonedDateTime.getDate().asCDATE();
               *((_CTIME*)pValue+7) = zonedDateTime.getTime().asCTIME();
               }

            if (pDuration != NULL)
               {
               p = (CtShort*)((_CDATE*)pValue+8);
               *p = pDuration->getDays();
               *(p+1) = pDuration->getWeeks();
               *(p+2) = pDuration->getMonths();
               *(p+3) = pDuration->getYears();
               }
            }

         break;
         }
      case 'V':
         {
         CtString
            string;
         CcbbInterval
            interval;
         CcbbZonedDateTime
            zonedDateTime;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         interval.fromParameter(string);

         zonedDateTime = interval.start();
         if (zonedDateTime == CM_MIN_ZONED_DATETIME)
            *((double*)pValue) = 0;
         else
            {
            *((_CDATE*)pValue) = zonedDateTime.getDate().asCDATE();
            *((_CTIME*)pValue+1) = zonedDateTime.getTime().asCTIME();
            }
         zonedDateTime = interval.end();
         if (zonedDateTime == CM_MAX_ZONED_DATETIME)
            *((double*)pValue+2) = 0;
         else
            {
            *((_CDATE*)pValue+2) = zonedDateTime.getDate().asCDATE();
            *((_CTIME*)pValue+3) = zonedDateTime.getTime().asCTIME();
            }
         break;
         }
      #endif
      case 'T':
         {
         CtTime
            time;

         rc = CgPO->getParameterItem(*pParmHandle,pName,time,pState);
         if (time == CtTime(0))
            *((long*)pValue) = 0;
         else
            *((_CTIME*)pValue) = time.asCTIME();
         break;
         }
      case 'D':
         {
         CtDate
            date;

         rc = CgPO->getParameterItem(*pParmHandle,pName,date,pState);

         if (date == CtDate(1,CtDate::January,1))
            *((long*)pValue) = 0;
         else
            *((_CDATE*)pValue) = date.asCDATE();
         break;
         }
      case 'd':
         {
         CtString
            string;
         CcabListDate*
            list = new CcabListDate();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (rc == 0) list->fromParameter(string);

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      case 't':
         {
         CtString
            string;
         CcabListTime*
            list = new CcabListTime();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (rc == 0) list->fromParameter(string);

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      #ifdef INC_BB
      case 'M':
         {
         CtString
            string;
         CcbbTranslatableString
            tstring;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         tstring.fromParameter(string);

         *((CtMri*) pValue) = tstring.asCode();

         break;
         }
      #endif
      case 'C':
         {
         CtString
            string;
         CcabCOID
            coid;

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (string.size() != 0) {
            coid.fromParameter(string);
            string = coid.platformID()+":"+coid.classID()+":"+coid.uniqueID();
         }

         strcpy((CtChar*) pValue,(CtChar*) string);

         break;
         }
      case 'c':
         {
         CtString
            string;
         CcabListCOID*
            list = new CcabListCOID();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);

         if (string.size() != 0) list->fromParameter(string);

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      case 'B':
         {
         CtBoolean
            boolean;

         rc = CgPO->getParameterItem(*pParmHandle,pName,boolean,pState);

         *((CtBoolean*) pValue) = boolean;

         break;
         }
      case 'p':
         {
         CtString
            string;
         CcabListParmCon*
            list = new CcabListParmCon();

         rc = CgPO->getParameterItem(*pParmHandle,pName,string,pState);
   #ifdef INC_DUMP
   ofstream fout("e:\\ap.out",append);
   fout << "Before restore " << endl;
   fout << "String from server: " << string << "***" << endl;
//   if (string.size() == 0)
//      pParmHandle->print(fout, 1);
   fout.close();
   #endif
         if (string.size() != 0)
           list->fromParameter(string);

   #ifdef INC_DUMP
   fout.open("e:\\ap.out",append);
   fout << "After restore" << endl;
   fout.close();
   #endif

         *((CtDWord*)pValue) = (CtDWord)list;

         break;
         }
      case 'r':
         {
         CcabParameterContainerHandle*
            p = new CcabParameterContainerHandle();

         rc = CgPO->getParameterItem(*pParmHandle,pName,*p,pState);

         *((CtDWord*)pValue) = (CtDWord)p;

         break;
         }
      default:
         {
         rc = APSL_ERROR;
         break;
         }
      }
   if (rc != APSL_ERROR)
      {
      if (*((CtInteger*)pState) != 0)
         {
   #ifdef INC_DUMP
   fout.open("e:\\ap.out",append);
   fout << "pState not null!" << endl;
//   pState->print(fout, 1);
   fout << endl;
   fout.close();
   #endif
         strcpy((CtChar*)pBuffer, (CtChar*)pState->diagMsgRef());
         }
      }
   }
delete pState;
return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_stringValue(
   CtChar* pParmString,
   CtChar* pPatternString,
   CtChar* pString)
////////////////////////////////////////////////////////////////////////////////
{
CtShort rc;
rc = APSL_OK;

CcbbDynTString dTString;
dTString.fromParameter(pParmString);

   #ifdef INC_DUMP
   ofstream fout4("e:\\ap.out",append);
   fout4 << "Raw string: " << pParmString << endl;
   fout4 << "From asParameter(): " << dTString.asParameter() << endl;
   fout4.close();
   #endif

strcpy(pString, dTString.stringify(CC_NULL_STRING,pPatternString));
return (rc);
}


////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_newList(
   CtDWord* pList,
   CtChar pType)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   switch (pType)
      {
      case 'i':
         {
         CcabListInteger* p = new CcabListInteger();
         *pList = (CtDWord)p;
         break;
         }
      case 's':
         {
         CcabListString* p = new CcabListString();
         *pList = (CtDWord)p;
         break;
         }
      case 'd':
         {
         CcabListDate* p = new CcabListDate();
         *pList = (CtDWord)p;
         break;
         }
      case 't':
         {
         }
      case 'm':
         {
         }
      case 'c':
         {
         CcabListCOID* p = new CcabListCOID();
         *pList = (CtDWord)p;
         break;
         }
      case 'p':
         {
         CcabListParmCon* p = new CcabListParmCon();
         *pList = (CtDWord)p;
         break;
         }
      default:
         {
         }
      }
   }

return (rc);
}

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_getListSize(
   CtVoid* pList,
   CtInteger* pSize,
   CtChar pType)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   switch (pType)
      {
      case 'i':
         {
         CcabListInteger*
            list = (CcabListInteger*)pList;

         *pSize = list->count();

         break;
         }
      case 's':
         {
         CcabListString*
            list = (CcabListString*)pList;

         *pSize = list->count();

         break;
         }
      case 'd':
         {
         CcabListDate*
            list = (CcabListDate*)pList;

         *pSize = list->count();

         break;
         }
      case 't':
         {
         }
      case 'm':
         {
         }
      case 'c':
         {
         CcabListCOID*
            list = (CcabListCOID*)pList;

         *pSize = list->count();

         break;
         }
      case 'p':
         {
         CcabListParmCon*
            list = (CcabListParmCon*)pList;

         *pSize = list->count();

         break;
         }
      default:
         {
         }
      }
   }

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_addListItem(
   CtVoid* pList,
   CtVoid* pValue,
   CtChar pType)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (pList == NULL)
   {
   rc = APSL_INVALID_LIST;
   }
else
   {
   switch (pType)
      {
      case 'i':
         {
         CcabListInteger*
            list = (CcabListInteger*)pList;
         CtInteger
            item = *((CtInteger*)pValue);

         list->add(item);

         break;
         }
      case 's':
         {
         CcabListString*
            list = (CcabListString*)pList;
         CtString
            item((CtChar*)pValue);

         list->add(item);

         break;
         }
      case 'd':
         {
         CcabListDate*
            list = (CcabListDate*)pList;
         CtDate
            item(*((_CDATE*)pValue));

         list->add(item);

         break;
         }
      case 't':
         {
         }
      case 'm':
         {
         }
      case 'c':
         {
         CcabListCOID*
            list = (CcabListCOID*)pList;
         CtChar
            record[APSL_REC_SZ];
         CtChar
            *platformID,
            *classID,
            *uniqueID;

         strcpy(record,(CtChar*) pValue);

         platformID = strtok(record,":");
         classID    = strtok(NULL,":");
         uniqueID   = strtok(NULL,":");

         CcabCOID
            item(platformID,classID,uniqueID);

         list->add(item);

         break;
         }
      case 'r':
         {
         CcabListParmCon*
            list = (CcabListParmCon*)pList;
         CcabParameterContainerHandle
            item = *((CcabParameterContainerHandle*)pValue);
         list->add(item);

         break;
         }
      default:
         {
         }
      }
   }

return (rc);
}

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_getListItem(
   CtVoid* pList,
   CtInteger pIndex,
   CtVoid* pValue,
   CtChar pType)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;

rc = APSL_OK;

if (pList == NULL)
   {
   rc = APSL_INVALID_LIST;
   }
else
   {
   switch (pType)
      {
      case 'i':
         {
         CcabListInteger*
            list = (CcabListInteger*)pList;
         CtInteger
            item = list->operator[](pIndex);

         *((CtInteger*)pValue) = item;

         break;
         }
      case 's':
         {
         CcabListString*
            list = (CcabListString*)pList;
         CtString
            item = list->operator[](pIndex);

         strcpy((CtChar*)pValue,(CtChar*)item);

         break;
         }
      case 'd':
         {
         CcabListDate*
            list = (CcabListDate*)pList;
         CtDate
            item = list->operator[](pIndex);
         *((_CDATE*)pValue) = item.asCDATE();
         break;
         }
      case 't':
         {
         }
      case 'm':
         {
         }
      case 'c':
         {
         CcabListCOID*
            list = (CcabListCOID*)pList;
         CcabCOID
            item = list->operator[](pIndex);
         CtString
            string = item.platformID()+":"+item.classID()+":"+item.uniqueID();

         strcpy((CtChar*)pValue,(CtChar*)string);

         break;
         }
      #ifdef INC_BB
      case 'v':
         {
         CcabListString*
            list = (CcabListString*)pList;
         CtString
            item = list->operator[](pIndex);
         CtDate
            start,
            end;
         CtString
            string;
         CcbbInterval
            interval;

         interval.fromParameter(item);
         *((_CDATE*)pValue) =
            interval.start().getDate().asCDATE();
         *(((_CDATE*)pValue + sizeof(_CDATE))) =
            interval.end().getDate().asCDATE();
         break;
         }
      #endif
      case 'p':
      case 'r':
         {
         CcabListParmCon*
            list = (CcabListParmCon*)pList;
         CcabParameterContainerHandle*
            item = new CcabParameterContainerHandle();

         *item = list->operator[](pIndex);

         *((CtDWord*)pValue) = (CtDWord)item;

         break;
         }
      default:
         {
         }
      }
   }

return (rc);
}

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_sendNew(
   CtChar* pSelector,
   CtChar* pPlatformID,
   CtChar* pClassID,
   CtChar* pUniqueID,
   CcabParameterContainerHandle* pParmHandle,
   CcabNodeContainerHandle* pContext,
   CcabTargetQueueHandle* pTarget)
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc,rc1;


rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {
   CcabCOID coid(pPlatformID, pClassID, pUniqueID);

   CcabNodeContainerHandle
      nodeContainerH;
   CcabTargetQueueHandle
      targetQueueH;

   if (pContext == NULL)
      {
      nodeContainerH = CgPO->newNodeContainer();
      rc1 = 2;
      }
   else
      {
      nodeContainerH = *pContext;
      rc1 = 1;
      }

   if (pTarget == NULL)
      targetQueueH = CgPO->newTargetQueue();
   else
      targetQueueH = *pTarget;

   #ifdef INC_DUMP
   ITime t1;
   ofstream fout1("e:\\ap.out",append);
   fout1 << "======== Send ========" << endl;
   fout1 << "T1: " << t1.now() << endl;
   fout1.close();
   #endif

   CgPO->sendNew(
      pSelector,
      coid,
      *pParmHandle,
      nodeContainerH,
      TRUE,
      "",
      targetQueueH,
      CgPO->newTargetQueue());
   }

   CgPO->flushSentMessages();

   #ifdef INC_DUMP
   ofstream fout2("e:\\ap.out",append);
   ITime t2;
   fout2 << "T2: " << t2.now() << endl;
   fout2.close();
   #endif

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_receive()
////////////////////////////////////////////////////////////////////////////////
{
CtShort
   rc;
const CcabCoarseMessageHandle
   CC_CURRENT_MESSAGE(CE_CURRENT_MESSAGE);

rc = APSL_OK;

if (CgPO == NULL)
   {
   rc = APSL_NOT_LOADED;
   }
else
   {

   #ifdef INC_DUMP
   ITime t3;
   ofstream fout3("e:\\ap.out",append);
   fout3 << "====== Receive =======" << endl;
   fout3 << "T3: " << t3.now() << endl;
   fout3.close();
   #endif

   if (CgPO->receive(CgPO->targetCOID().platformID(), NULL))
      {
      rc = APSL_RECEIVE_ERROR;
      }
   }

   #ifdef INC_DUMP
   ITime t4;
   ofstream fout4("e:\\ap.out",append);
   fout4 << "T4: " << t4.now() << endl;
   fout4.close();
   #endif

   CcabTrace::traceLevel(5);
   CC_CURRENT_MESSAGE.trace();

return (rc);
};

////////////////////////////////////////////////////////////////////////////////
CtError CBCENTRY
apsl_retryReceive()
////////////////////////////////////////////////////////////////////////////////
{
return apsl_receive();
};
