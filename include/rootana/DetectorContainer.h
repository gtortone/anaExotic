#ifndef DETECTORCONTAINER_H_
#define DETECTORCONTAINER_H_

#include "utils/Exotic.h"
#include <map>
#include <string>
#include <vector>

class DetectorContainer {

 private:
   std::map<std::string, exotic::Dsssd *> DsssdMap;
   //std::map<std::string, exotic::Ppac *> PpacMap;

   std::vector<exotic::Dsssd *> DsssdList;
   //std::vector<exotic::Ppac *> PpacList;

 public:
   DetectorContainer(){};
   ~DetectorContainer() {
      clear();
   };

   void clear(void) {
      DsssdList.clear();
      //DsssdList.shrink_to_fit();
      //PpacList.clear();
      //PpacList.shrink_to_fit();
      DsssdMap.clear();
      //PpacMap.clear();
   }

   void create(std::string type, std::string name) {
      if (type == "dsssd") {
         DsssdMap[name] = new exotic::Dsssd(name);
         DsssdList.push_back(DsssdMap[name]);
      } //else if (type == "ppac") {
      //   PpacMap[name] = new exotic::Ppac(name);
      //   PpacList.push_back(PpacMap[name]);
      //}
   }

   exotic::Dsssd *getDsssd(std::string name) {
      return DsssdMap[name];
   }

   //exotic::Ppac *getPpac(std::string name) {
   //   return PpacMap[name];
   //}

   int DsssdSize(void) {
      return DsssdList.size();
   }

   exotic::Dsssd *getDsssd(int i) {
      if (i < DsssdSize())
         return DsssdList[i];
      return NULL;
   }
};

#endif