#ifndef DETECTORCONTAINER_H_
#define DETECTORCONTAINER_H_

#include "utils/Exotic.h"
#include <map>
#include <string>
#include <vector>

class DetectorContainer {

 private:
   std::map<std::string, exotic::Dsssd *> DsssdMap;
   std::map<std::string, exotic::Mcp *> McpMap;

   std::vector<exotic::Dsssd *> DsssdList;
   std::vector<exotic::Mcp *> McpList;

 public:
   DetectorContainer(){};
   ~DetectorContainer() {
      clear();
   };

   void clear(void) {
      DsssdList.clear();
      //DsssdList.shrink_to_fit();
      McpList.clear();
      //McpList.shrink_to_fit();
      DsssdMap.clear();
      McpMap.clear();
   }

   void create(std::string type, std::string name) {
      if (type == "dsssd") {
         DsssdMap[name] = new exotic::Dsssd(name);
         DsssdList.push_back(DsssdMap[name]);
      } else if (type == "mcp") {
         McpMap[name] = new exotic::Mcp(name);
         McpList.push_back(McpMap[name]);
      }
   }

   exotic::Dsssd *getDsssd(std::string name) {
      return DsssdMap[name];
   }

   exotic::Mcp *getMcp(std::string name) {
      return McpMap[name];
   }

   int DsssdSize(void) {
      return DsssdList.size();
   }

   int McpSize(void) {
      return McpList.size();
   }

   exotic::Dsssd *getDsssd(int i) {
      if (i < DsssdSize())
         return DsssdList[i];
      return NULL;
   }
   
   exotic::Mcp *getMcp(int i) {
      if (i < McpSize())
         return McpList[i];
      return NULL;
   }
};

#endif