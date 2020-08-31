#pragma once

#include "Database.h"
#include "Database_join_iterator.h"
#include "Datum.h"
#include "Filter.h"
#include "linear_map.h"
#include "linear_set.h"
#include "Record.h"
#include "string_map.h"
#include "Table.h"

using Records = std::list<Record>;
using RecordSet = Db::Types::linear_set<Record>;
using DatumToRecordsMap = std::map<Datum, RecordSet>;
using DatumToRecordsStringMap = Db::Types::string_map<DatumToRecordsMap>;
using Filters = Db::Types::linear_set<Filter>;
