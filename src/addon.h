#include <map>
#include <string>
#include <sstream>
#include <nan.h>
#include <node.h>
#include <windows.h>
#include <queue>
#include <ntstatus.h>
#include "SimConnect.h"

using namespace v8;

struct CallbackData {
	DWORD cbData;
	SIMCONNECT_RECV* pData;
	NTSTATUS ntstatus;
};

struct SystemEventRequest {
	Nan::Callback* jsCallback;
};

struct DataDefinition {
	SIMCONNECT_DATA_DEFINITION_ID id;
	unsigned int num_values;
	std::vector<std::string> datum_names;
	std::vector<SIMCONNECT_DATATYPE> datum_types;
};


std::map<SIMCONNECT_EXCEPTION, const char*> exceptionNames = {
	{ SIMCONNECT_EXCEPTION_NONE, "SIMCONNECT_EXCEPTION_NONE" },
	{ SIMCONNECT_EXCEPTION_ERROR, "SIMCONNECT_EXCEPTION_ERROR" },
	{ SIMCONNECT_EXCEPTION_SIZE_MISMATCH, "SIMCONNECT_EXCEPTION_SIZE_MISMATCH" },
	{ SIMCONNECT_EXCEPTION_UNRECOGNIZED_ID, "SIMCONNECT_EXCEPTION_UNRECOGNIZED_ID" },
	{ SIMCONNECT_EXCEPTION_UNOPENED, "SIMCONNECT_EXCEPTION_UNOPENED" },
	{ SIMCONNECT_EXCEPTION_VERSION_MISMATCH, "SIMCONNECT_EXCEPTION_VERSION_MISMATCH" },
	{ SIMCONNECT_EXCEPTION_TOO_MANY_GROUPS, "SIMCONNECT_EXCEPTION_TOO_MANY_GROUPS" },
	{ SIMCONNECT_EXCEPTION_NAME_UNRECOGNIZED, "SIMCONNECT_EXCEPTION_NAME_UNRECOGNIZED" },
	{ SIMCONNECT_EXCEPTION_TOO_MANY_EVENT_NAMES, "SIMCONNECT_EXCEPTION_TOO_MANY_EVENT_NAMES" },
	{ SIMCONNECT_EXCEPTION_EVENT_ID_DUPLICATE, "SIMCONNECT_EXCEPTION_EVENT_ID_DUPLICATE" },
	{ SIMCONNECT_EXCEPTION_TOO_MANY_MAPS, "SIMCONNECT_EXCEPTION_TOO_MANY_MAPS" },
	{ SIMCONNECT_EXCEPTION_TOO_MANY_OBJECTS, "SIMCONNECT_EXCEPTION_TOO_MANY_OBJECTS" },
	{ SIMCONNECT_EXCEPTION_TOO_MANY_REQUESTS, "SIMCONNECT_EXCEPTION_TOO_MANY_REQUESTS" },
	{ SIMCONNECT_EXCEPTION_WEATHER_INVALID_PORT, "SIMCONNECT_EXCEPTION_WEATHER_INVALID_PORT" },
	{ SIMCONNECT_EXCEPTION_WEATHER_INVALID_METAR, "SIMCONNECT_EXCEPTION_WEATHER_INVALID_METAR" },
	{ SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_GET_OBSERVATION, "SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_GET_OBSERVATION" },
	{ SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_CREATE_STATION, "SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_CREATE_STATION" },
	{ SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_REMOVE_STATION, "SIMCONNECT_EXCEPTION_WEATHER_UNABLE_TO_REMOVE_STATION" },
	{ SIMCONNECT_EXCEPTION_INVALID_DATA_TYPE, "SIMCONNECT_EXCEPTION_INVALID_DATA_TYPE" },
	{ SIMCONNECT_EXCEPTION_INVALID_DATA_SIZE, "SIMCONNECT_EXCEPTION_INVALID_DATA_SIZE" },
	{ SIMCONNECT_EXCEPTION_DATA_ERROR, "SIMCONNECT_EXCEPTION_DATA_ERROR" },
	{ SIMCONNECT_EXCEPTION_INVALID_ARRAY, "SIMCONNECT_EXCEPTION_INVALID_ARRAY" },
	{ SIMCONNECT_EXCEPTION_CREATE_OBJECT_FAILED, "SIMCONNECT_EXCEPTION_CREATE_OBJECT_FAILED" },
	{ SIMCONNECT_EXCEPTION_OPERATION_INVALID_FOR_OBJECT_TYPE, "SIMCONNECT_EXCEPTION_OPERATION_INVALID_FOR_OBJECT_TYPE" },
	{ SIMCONNECT_EXCEPTION_ILLEGAL_OPERATION, "SIMCONNECT_EXCEPTION_ILLEGAL_OPERATION" },
	{ SIMCONNECT_EXCEPTION_ALREADY_SUBSCRIBED, "SIMCONNECT_EXCEPTION_ALREADY_SUBSCRIBED" },
	{ SIMCONNECT_EXCEPTION_INVALID_ENUM, "SIMCONNECT_EXCEPTION_INVALID_ENUM" },
	{ SIMCONNECT_EXCEPTION_DEFINITION_ERROR, "SIMCONNECT_EXCEPTION_DEFINITION_ERROR" },
	{ SIMCONNECT_EXCEPTION_DUPLICATE_ID, "SIMCONNECT_EXCEPTION_DUPLICATE_ID" },
	{ SIMCONNECT_EXCEPTION_DATUM_ID, "SIMCONNECT_EXCEPTION_DATUM_ID" },
	{ SIMCONNECT_EXCEPTION_OUT_OF_BOUNDS, "SIMCONNECT_EXCEPTION_OUT_OF_BOUNDS" },
	{ SIMCONNECT_EXCEPTION_ALREADY_CREATED, "SIMCONNECT_EXCEPTION_ALREADY_CREATED" },
	{ SIMCONNECT_EXCEPTION_OBJECT_OUTSIDE_REALITY_BUBBLE, "SIMCONNECT_EXCEPTION_OBJECT_OUTSIDE_REALITY_BUBBLE" },
	{ SIMCONNECT_EXCEPTION_OBJECT_CONTAINER, "SIMCONNECT_EXCEPTION_OBJECT_CONTAINER" },
	{ SIMCONNECT_EXCEPTION_OBJECT_AI, "SIMCONNECT_EXCEPTION_OBJECT_AI" },
	{ SIMCONNECT_EXCEPTION_OBJECT_ATC, "SIMCONNECT_EXCEPTION_OBJECT_ATC" },
	{ SIMCONNECT_EXCEPTION_OBJECT_SCHEDULE, "SIMCONNECT_EXCEPTION_OBJECT_SCHEDULE" }
};

std::map<SIMCONNECT_DATATYPE, int> sizeMap =
{
	{ SIMCONNECT_DATATYPE_INT32, 4 },
	{ SIMCONNECT_DATATYPE_INT64, 8 },
	{ SIMCONNECT_DATATYPE_FLOAT32, 4 },
	{ SIMCONNECT_DATATYPE_FLOAT64, 8 },
	{ SIMCONNECT_DATATYPE_STRING8, 8 },
	{ SIMCONNECT_DATATYPE_STRING32, 32 },
	{ SIMCONNECT_DATATYPE_STRING64, 64 },
	{ SIMCONNECT_DATATYPE_STRING128, 128 },
	{ SIMCONNECT_DATATYPE_STRING256, 256 },
	{ SIMCONNECT_DATATYPE_STRING260, 260 }
};


void handleReceived_Data(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_DataByType(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_Event(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_Exception(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_Filename(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_Open(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_SystemState(Isolate* isolate, SIMCONNECT_RECV* pData, DWORD cbData);
void handleReceived_Quit(Isolate* isolate);
void handle_Error(Isolate* isolate, NTSTATUS code);

void messageReceiver(uv_async_t* handle);
DataDefinition generateDataDefinition(Isolate* isolate, HANDLE hSimConnect, Local<Array> requestedValues);