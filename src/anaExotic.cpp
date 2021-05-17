#include "rootana/Application.h"
#include "midas.h"
#include "midas/Event.h"

int main(int argc, char *argv[]) {

   midas::Event e;

   //
   // begin: generate fake event
   //
   
   EVENT_HEADER *pevent;
   WORD *pdata;
   WORD *pmegamp;

   /* init bank structure */
   pevent = (EVENT_HEADER *) malloc(1024);
   bm_compose_event(pevent, 1, 2, 0, 123);
   pdata = (WORD *)(pevent + 1);

   bk_init(pdata);
   bk_create(pdata, "M000", TID_WORD, (void **)&pmegamp);
   
   *pmegamp++ = 0x00CC; // EC
   *pmegamp++ = 0x02C2; // TSH
   *pmegamp++ = 0x55E6; // TSL
   *pmegamp++ = 0x0008; // NM
   *pmegamp++ = 0x0005; // NS

   for (int c = 0; c < 16; c++) {
      *pmegamp++ = c;
      *pmegamp++ = 0xA; // sample 1
      *pmegamp++ = 0xB; // sample 2
      *pmegamp++ = 0xC; // sample 3
      *pmegamp++ = 0xD; // sample 4
      *pmegamp++ = 0xE; // sample 5
   }

   bk_close(pdata, pmegamp);
   pevent->data_size = bk_size(pdata);
  
   //
   // end: generate fake event
   //

   memcpy(e.GetEventHeader(), pevent, sizeof(midas::Event::Header));
   memcpy(e.GetData(), pdata, bk_size(pdata));

   printf("sizeof(midas::Event::Header) = %ld\n", sizeof(midas::Event::Header));
   printf("e.GetEventId() = %d\n", e.GetEventId());
   printf("e.GetSerialNumber() = %d\n", e.GetSerialNumber());
   printf("e.SetBankList() = %d\n", e.SetBankList());
   printf("e.GetBankList() = %s\n", e.GetBankList());
   e.Print("a");

   std::shared_ptr<rootana::App> app (new rootana::App("rootana", &argc, argv));
	app->Run();

}
