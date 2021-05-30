#include "rootana/Application.h"
#include "midas.h"
#include "midas/Event.h"

int main(int argc, char *argv[]) {

   std::shared_ptr<rootana::App> app (new rootana::App("rootana", &argc, argv));
	app->Run();
}
