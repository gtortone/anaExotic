/********************************************************************\

  Name:         TMidasOnline.cxx
  Created by:   Konstantin Olchanski - TRIUMF

  Contents:     C++ MIDAS analyzer

  $Id$

\********************************************************************/

#include "TMidasOnline.h"

#include <string>
#include <assert.h>

//#include "midas.h"
#include "msystem.h"
//#include "hardware.h"
//#include "ybos.h"


TMidasOnline::TMidasOnline() // ctor
{
  fDB = 0;
  fStartHandler  = 0;
  fStopHandler   = 0;
  fPauseHandler  = 0;
  fResumeHandler = 0;
  fEventRequests = 0;
  fEventHandler  = 0;
}

TMidasOnline::~TMidasOnline() // dtor
{
  disconnect();
  assert(!"TMidasOnline::~TMidasOnline(): destruction of the TMidasOnline singleton is not permitted!");
}

TMidasOnline* TMidasOnline::instance()
{
  if (!gfMidas)
    gfMidas = new TMidasOnline();
  
  return gfMidas;
}

int TMidasOnline::connect(const char*hostname,const char*exptname,const char*progname)
{
  int status;
  
  char xhostname[HOST_NAME_LENGTH];
  char xexptname[NAME_LENGTH];
  
  /* get default from environment */
  status = cm_get_environment(xhostname, sizeof(xhostname), xexptname, sizeof(xexptname));
  assert(status == CM_SUCCESS);
  
  if (hostname)
    strlcpy(xhostname,hostname,sizeof(xhostname));
  
  if (exptname)
    strlcpy(xexptname,exptname,sizeof(xexptname));
  
  fHostname = xhostname;
  fExptname = xexptname;

  fprintf(stderr, "TMidasOnline::connect: Connecting to experiment \"%s\" on host \"%s\"\n", fExptname.c_str(), fHostname.c_str());
  
  //int watchdog = DEFAULT_WATCHDOG_TIMEOUT;
  int watchdog = 60*1000;

  status = cm_connect_experiment1((char*)fHostname.c_str(), (char*)fExptname.c_str(), (char*)progname, NULL, DEFAULT_ODB_SIZE, watchdog);
  
  if (status == CM_UNDEF_EXP)
    {
      fprintf(stderr, "TMidasOnline::connect: Error: experiment \"%s\" not defined.\n", fExptname.c_str());
      return -1;
    }
  else if (status != CM_SUCCESS)
    {
      fprintf(stderr, "TMidasOnline::connect: Cannot connect to MIDAS, status %d.\n", status);
      return -1;
    }
  
  status = cm_get_experiment_database(&fDB, NULL);
  assert(status == CM_SUCCESS);
  
  cm_set_watchdog_params(true, 60*1000);

  return 0;
}

int TMidasOnline::disconnect()
{
  if (fDB)
    {
      fprintf(stderr, "TMidasOnline::disconnect: Disconnecting from experiment \"%s\" on host \"%s\"\n", fExptname.c_str(), fHostname.c_str());
      cm_disconnect_experiment();
      fDB = 0;
    }
  
  return 0;
}

void TMidasOnline::registerTransitions()
{
  cm_register_transition(TR_START,  NULL, 300);
  cm_register_transition(TR_PAUSE,  NULL, 700);
  cm_register_transition(TR_RESUME, NULL, 300);
  cm_register_transition(TR_STOP,   NULL, 700);
}

void TMidasOnline::setTransitionHandlers(TransitionHandler start,TransitionHandler stop,TransitionHandler pause,TransitionHandler resume)
{
  fStartHandler  = start;
  fStopHandler   = stop;
  fPauseHandler  = pause;
  fResumeHandler = resume;
}

bool TMidasOnline::checkTransitions()
{
  int transition, run_number, trans_time;
  
  int status = cm_query_transition(&transition, &run_number, &trans_time);
  if (status != CM_SUCCESS)
    return false;
  
  //printf("cm_query_transition: status %d, tr %d, run %d, time %d\n",status,transition,run_number,trans_time);

  for (unsigned i=0; i<fHandlers.size(); i++)
    fHandlers[i]->Transition(transition, run_number, trans_time);
  
  if (transition == TR_START)
    {
      if (fStartHandler)
        (*fStartHandler)(transition,run_number,trans_time);
      return true;
    }
  else if (transition == TR_STOP)
    {
      if (fStopHandler)
        (*fStopHandler)(transition,run_number,trans_time);
      return true;
      
    }
  else if (transition == TR_PAUSE)
    {
      if (fPauseHandler)
        (*fPauseHandler)(transition,run_number,trans_time);
      return true;
      
    }
  else if (transition == TR_RESUME)
    {
      if (fResumeHandler)
        (*fResumeHandler)(transition,run_number,trans_time);
      return true;
    }
  
  return false;
}

bool TMidasOnline::poll(int mdelay)
{
  //printf("poll!\n");
  
  if (checkTransitions())
    return true;
  
  int status = cm_yield(mdelay);
  if (status == RPC_SHUTDOWN || status == SS_ABORT)
    {
      fprintf(stderr, "TMidasOnline::poll: cm_yield(%d) status %d, shutting down.\n",mdelay,status);
      disconnect();
      return false;
    }
  
  return true;
}

bool TMidasOnline::sleep(int mdelay)
{
  //printf("poll!\n");
  
  if (checkTransitions())
    return true;
  
  int status = ss_suspend(mdelay, MSG_BM);
  if (status == SS_SUCCESS)
    return true;
  if (status == SS_TIMEOUT)
    return true;
#if 0
  if (status == SS_SERVER_RECV) {
    //printf("ss_suspend status %d\n", status);
    // FIXME: maybe sleep here?
    return true;
  }
#endif
  fprintf(stderr, "TMidasOnline::sleep(): Unexpected ss_suspend() status %d\n", status);
#if 0
  if (status == RPC_SHUTDOWN || status == SS_ABORT)
    {
      fprintf(stderr, "TMidasOnline::poll: cm_yield(%d) status %d, shutting down.\n",mdelay,status);
      disconnect();
      return false;
    }
#endif
  
  return true;
}

void TMidasOnline::setEventHandler(EventHandler handler)
{
  fEventHandler  = handler;
}

static void eventCallback(HNDLE buffer_handle, HNDLE request_id, EVENT_HEADER* pheader, void* pevent)
{
#if 0
  printf("eventCallback: buffer %d, request %d, pheader %p (event_id: %d, trigger mask: 0x%x, serial: %d, time: %d, size: %d), pevent %p\n",
	 buffer_handle,
	 request_id,
	 pheader,
	 pheader->event_id,
	 pheader->trigger_mask,
	 pheader->serial_number,
	 pheader->time_stamp,
	 pheader->data_size,
	 pevent);
#endif

  TMidasOnline* midas = TMidasOnline::instance();
  
  for (unsigned i=0; i<midas->fHandlers.size(); i++)
    midas->fHandlers[i]->Event(pheader, sizeof(EVENT_HEADER) + pheader->data_size);

  if (midas->fEventHandler)
    midas->fEventHandler(pheader,pevent,pheader->data_size);
}

int TMidasOnline::receiveEvent(int requestId, void* pevent, int size, bool async)
{
  EventRequest* r = fEventRequests;

  while (1)
    {
      if (!r)
        {
          fprintf(stderr, "TMidasOnline::receiveEvent: Cannot find request %d\n", requestId);
          return -1;
        }

      if (r->fRequestId == requestId)
        break;

      r = r->fNext;
    }

  int flag = 0;
  if (async){
#ifdef BM_NO_WAIT
    flag = BM_NO_WAIT;
#else
    flag = ASYNC;
#endif
  }


  int status = bm_receive_event(r->fBufferHandle, pevent, &size, flag);

  if (status == BM_ASYNC_RETURN)
    {
      return 0;
    }

  if (status != BM_SUCCESS)
    {
      fprintf(stderr, "TMidasOnline::receiveEvent: bm_receive_event() error %d\n", status);
      return -1;
    }

  return size;
}

#ifndef EVENT_BUFFER_SIZE
#define EVENT_BUFFER_SIZE 0
#endif

int TMidasOnline::eventRequest(const char* bufferName, int eventId, int triggerMask, int samplingType, bool poll)
{
  int status;
  EventRequest* r = new EventRequest();

  if (bufferName == NULL)
    bufferName = EVENT_BUFFER_NAME;
  
  r->fNext         = NULL;
  r->fBufferName   = bufferName;
  r->fEventId      = eventId;
  r->fTriggerMask  = triggerMask;
  r->fSamplingType = samplingType;
  
  
  /*---- open event buffer ---------------------------------------*/
  status = bm_open_buffer((char*)bufferName, EVENT_BUFFER_SIZE, &r->fBufferHandle);
  if (status!=SUCCESS && status!=BM_CREATED)
    {
      fprintf(stderr, "TMidasOnline::eventRequest: Cannot find data buffer \"%s\", bm_open_buffer() error %d\n", bufferName, status);
      return -1;
    }
  
  /* set the default buffer cache size (but not GET_RECENT sampling type*/
	if(samplingType != GET_RECENT){
		status = bm_set_cache_size(r->fBufferHandle, 100000, 0);
		assert(status == BM_SUCCESS);
	}

  if (poll)
    status = bm_request_event(r->fBufferHandle, r->fEventId, r->fTriggerMask, r->fSamplingType, &r->fRequestId, NULL);
  else
    status = bm_request_event(r->fBufferHandle, r->fEventId, r->fTriggerMask, r->fSamplingType, &r->fRequestId, eventCallback);
  assert(status == BM_SUCCESS);
  
  fprintf(stderr, "TMidasOnline::eventRequest: Event request: buffer \"%s\" (%d), event id 0x%x, trigger mask 0x%x, sample %d, request id: %d\n",bufferName,r->fBufferHandle,r->fEventId,r->fTriggerMask,r->fSamplingType,r->fRequestId);
  
  r->fNext = fEventRequests;
  fEventRequests = r;
  
  return r->fRequestId;
};

int TMidasOnline::getBufferLevel(){

  if(!fEventRequests || !fEventRequests->fBufferHandle) return -1;

  int n_bytes;
  bm_get_buffer_level(fEventRequests->fBufferHandle, &n_bytes);

  return n_bytes;

}

int TMidasOnline::getBufferSize(){

  if(!fEventRequests || !fEventRequests->fBufferHandle) return -1;

  BUFFER_HEADER buffer_header;
  bm_get_buffer_info(fEventRequests->fBufferHandle,&buffer_header);      

  return buffer_header.size;

}
void TMidasOnline::deleteEventRequest(int requestId)
{
  for (EventRequest* r = fEventRequests; r != NULL; r = r->fNext)
    if (r->fRequestId == requestId)
      {
	int status = bm_delete_request(r->fRequestId);
	assert(status == BM_SUCCESS);
	
	r->fBufferHandle = -1;
	r->fRequestId    = -1;
      }
}

TMHandlerInterface::~TMHandlerInterface()
{
}

void TMidasOnline::RegisterHandler(TMHandlerInterface* h)
{
  fHandlers.push_back(h);
}

TMidasOnline* TMidasOnline::gfMidas = NULL;

//end
