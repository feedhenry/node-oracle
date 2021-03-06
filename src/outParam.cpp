
#include "outParam.h"
#include "nodeOracleException.h"
#include <iostream>
using namespace std;

Persistent<FunctionTemplate> OutParam::constructorTemplate;

void OutParam::Init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(New);
  constructorTemplate = Persistent<FunctionTemplate>::New(t);
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  constructorTemplate->SetClassName(String::NewSymbol("OutParam"));
  target->Set(String::NewSymbol("OutParam"), constructorTemplate->GetFunction());
}

Handle<Value> OutParam::New(const Arguments& args) {
  HandleScope scope;
  OutParam *outParam = new OutParam();

  outParam->_type = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  
  if (!args[1]->IsUndefined()) {
    REQ_OBJECT_ARG(1, opts);
    OBJ_GET_NUMBER(opts, "size", outParam->_size, 200);

    // check if there's an 'in' param
    if (opts->Has(String::New("in"))) {
      outParam->_inOut.hasInParam = true;
      switch(outParam->_type) {
      case OutParam::OCCIINT: {
        OBJ_GET_NUMBER(opts, "in", outParam->_inOut.intVal, 0);
        break;
      }
      case OutParam::OCCIDOUBLE: {
        OBJ_GET_NUMBER(opts, "in", outParam->_inOut.doubleVal, 0);
        break;
      }
      case OutParam::OCCIFLOAT: {
        OBJ_GET_NUMBER(opts, "in", outParam->_inOut.floatVal, 0);
        break;
      }
      case OutParam::OCCINUMBER: {
        OBJ_GET_NUMBER(opts, "in", outParam->_inOut.numberVal, 0);
        break;
      }
      case OutParam::OCCISTRING: {
        OBJ_GET_STRING(opts, "in", outParam->_inOut.stringVal);
        break;
      }
      default:
        throw NodeOracleException("Unhandled OutPram type!");
      }
    }
  }
  outParam->Wrap(args.This());
  return args.This();
}

OutParam::OutParam() {
  _inOut.hasInParam = false;
  _size = 200;
}

OutParam::~OutParam() {
}

int OutParam::type() {
  return _type;
}

int OutParam::size() {
  return _size;
}
