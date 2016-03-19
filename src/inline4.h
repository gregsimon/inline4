
#ifndef __inline4_h__
#define __inline4_h__

#include "v8.h"
#include "libplatform/libplatform.h"

#include "duktape.h"

#include <string>
#include <map>
#include <stack>

#include <assert.h>


#define I4T  printf("[%s]\n",__PRETTY_FUNCTION__)

// Shared base class for internal representations of v8.h client objects.
// This is required so we can track these instances via handlescope.
class iBase {
public:
  iBase() = delete;
  iBase(int t=DUK_TYPE_UNDEFINED, void* ptr=0) : _type(t), _ptr(ptr) {}
  virtual ~iBase() { _ptr=0; }
  int _type; // DUK_TYPE_*
  void* _ptr;
};

// Internal representations of v8.h client objects

class iString : public iBase {
public:
  iString() : iBase(DUK_TYPE_STRING) {I4T;}
  iString(const char* data) : iBase(DUK_TYPE_STRING), s(data) {I4T;}
  ~iString() {I4T;}
  int len() const { return s.size(); }
  std::string s;
};

class iScript : public iBase {
public:
  iScript() : iBase(DUK_TYPE_UNDEFINED), _bytecode(0), _len(0) {I4T;}
  ~iScript() {I4T; delete[] _bytecode; }
  bool set(const uint8_t* b, size_t l) {
    delete[] _bytecode;
   if( !(_bytecode = new uint8_t[l]))
    return false;
    memcpy(_bytecode, b, l);
    _len = l;
    return true;
  }
  void print() const {
    printf("bytecode %ld bytes:\n", _len);
    for (size_t i=0 ;i<_len; i++)
      printf("%02x ", _bytecode[i]);
    printf("\n");
  }
  size_t len() const { return _len; }
  const uint8_t* bytecode() { return _bytecode; }
private:
  uint8_t* _bytecode;
  size_t _len;
};

class iContext : public iBase {
public:
  iContext() : iBase(DUK_TYPE_UNDEFINED) {I4T;}
  ~iContext() {I4T;}
  duk_context* ctx;
};

class iIsolate : public iBase {
public:
  iIsolate() : iBase(DUK_TYPE_UNDEFINED), ctx(0) {I4T;}
  ~iIsolate() {I4T;}
  duk_context* ctx;
};

class iFunctionTemplate : public iBase {
public:
  iFunctionTemplate() : iBase(DUK_TYPE_UNDEFINED) {I4T;}
  int foo;
};

// Holds properties of that we'll apply to every instance of this
// template.
class iObjectTemplate : public iBase {
public:
  iObjectTemplate() : iBase(DUK_TYPE_UNDEFINED) {I4T;}
  std::map<std::string, iFunctionTemplate*> props;
};

namespace v8 {

class FunctionTemplate;

class Utils {
 public:
  static inline bool ApiCheck(bool condition,
                              const char* location,
                              const char* message) {
    if (!condition) Utils::ReportApiFailure(location, message);
    return condition;
  }

  template<class To>
  static inline Local<To> Convert(iBase* obj) {
    return Local<To>(reinterpret_cast<To*>(obj));
  }

/*
  static Local<FunctionTemplate> ToFunctionTemplate(NeanderObject obj);
  static Local<ObjectTemplate> ToObjectTemplate(NeanderObject obj);

  static inline Local<Context> ToLocal(
      v8::internal::Handle<v8::internal::Context> obj);
  static inline Local<Value> ToLocal(
      v8::internal::Handle<v8::internal::Object> obj);
  static inline Local<Name> ToLocal(
      v8::internal::Handle<v8::internal::Name> obj);
  static inline Local<String> ToLocal(
      v8::internal::Handle<v8::internal::String> obj);
  static inline Local<Symbol> ToLocal(
      v8::internal::Handle<v8::internal::Symbol> obj);
  static inline Local<RegExp> ToLocal(
      v8::internal::Handle<v8::internal::JSRegExp> obj);
  static inline Local<Object> ToLocal(
      v8::internal::Handle<v8::internal::JSReceiver> obj);
  static inline Local<Object> ToLocal(
      v8::internal::Handle<v8::internal::JSObject> obj);
  static inline Local<Array> ToLocal(
      v8::internal::Handle<v8::internal::JSArray> obj);
  static inline Local<Map> ToLocal(
      v8::internal::Handle<v8::internal::JSMap> obj);
  static inline Local<Set> ToLocal(
      v8::internal::Handle<v8::internal::JSSet> obj);
  static inline Local<Proxy> ToLocal(
      v8::internal::Handle<v8::internal::JSProxy> obj);
  static inline Local<ArrayBuffer> ToLocal(
      v8::internal::Handle<v8::internal::JSArrayBuffer> obj);
  static inline Local<ArrayBufferView> ToLocal(
      v8::internal::Handle<v8::internal::JSArrayBufferView> obj);
  static inline Local<DataView> ToLocal(
      v8::internal::Handle<v8::internal::JSDataView> obj);
  static inline Local<TypedArray> ToLocal(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Uint8Array> ToLocalUint8Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Uint8ClampedArray> ToLocalUint8ClampedArray(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Int8Array> ToLocalInt8Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Uint16Array> ToLocalUint16Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Int16Array> ToLocalInt16Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Uint32Array> ToLocalUint32Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Int32Array> ToLocalInt32Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Float32Array> ToLocalFloat32Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);
  static inline Local<Float64Array> ToLocalFloat64Array(
      v8::internal::Handle<v8::internal::JSTypedArray> obj);

  static inline Local<SharedArrayBuffer> ToLocalShared(
      v8::internal::Handle<v8::internal::JSArrayBuffer> obj);

  static inline Local<Message> MessageToLocal(
      v8::internal::Handle<v8::internal::Object> obj);
  static inline Local<Promise> PromiseToLocal(
      v8::internal::Handle<v8::internal::JSObject> obj);
  static inline Local<StackTrace> StackTraceToLocal(
      v8::internal::Handle<v8::internal::JSArray> obj);
  static inline Local<StackFrame> StackFrameToLocal(
      v8::internal::Handle<v8::internal::JSObject> obj);
  static inline Local<Number> NumberToLocal(
      v8::internal::Handle<v8::internal::Object> obj);
  static inline Local<Integer> IntegerToLocal(
      v8::internal::Handle<v8::internal::Object> obj);
  static inline Local<Uint32> Uint32ToLocal(
      v8::internal::Handle<v8::internal::Object> obj);
  static inline Local<FunctionTemplate> ToLocal(
      v8::internal::Handle<v8::internal::FunctionTemplateInfo> obj);
  static inline Local<ObjectTemplate> ToLocal(
      v8::internal::Handle<v8::internal::ObjectTemplateInfo> obj);
  static inline Local<Signature> SignatureToLocal(
      v8::internal::Handle<v8::internal::FunctionTemplateInfo> obj);
  static inline Local<AccessorSignature> AccessorSignatureToLocal(
      v8::internal::Handle<v8::internal::FunctionTemplateInfo> obj);
  static inline Local<External> ExternalToLocal(
      v8::internal::Handle<v8::internal::JSObject> obj);
  static inline Local<NativeWeakMap> NativeWeakMapToLocal(
      v8::internal::Handle<v8::internal::JSWeakMap> obj);
  static inline Local<Function> CallableToLocal(
      v8::internal::Handle<v8::internal::JSReceiver> obj);

#define DECLARE_OPEN_HANDLE(From, To) \
  static inline v8::internal::Handle<v8::internal::To> \
      OpenHandle(const From* that, bool allow_empty_handle = false);

OPEN_HANDLE_LIST(DECLARE_OPEN_HANDLE)

#undef DECLARE_OPEN_HANDLE

  template<class From, class To>
  static inline Local<To> Convert(v8::internal::Handle<From> obj) {
    DCHECK(obj.is_null() || !obj->IsTheHole());
    return Local<To>(reinterpret_cast<To*>(obj.location()));
  }

  template <class T>
  static inline v8::internal::Handle<v8::internal::Object> OpenPersistent(
      const v8::Persistent<T>& persistent) {
    return v8::internal::Handle<v8::internal::Object>(
        reinterpret_cast<v8::internal::Object**>(persistent.val_));
  }

  template <class T>
  static inline v8::internal::Handle<v8::internal::Object> OpenPersistent(
      v8::Persistent<T>* persistent) {
    return OpenPersistent(*persistent);
  }

  template <class From, class To>
  static inline v8::internal::Handle<To> OpenHandle(v8::Local<From> handle) {
    return OpenHandle(*handle);
  }*/

 private:
  static void ReportApiFailure(const char* location, const char* message);
};

} // namespace v8

#endif