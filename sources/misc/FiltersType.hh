#ifndef FILTERSTYPE_HH_
#define FILTERSTYPE_HH_

enum FilterKey {
  IP_SRC,
  IP_DEST,
  MAC_SRC,
  MAC_DEST,
  PROTOCOLS,
};

typedef std::map<FilterKey, std::vector<std::string> > Filters;

#endif
