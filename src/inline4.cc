

#include "inline4.h"





// -------------------------------------------------------------------------
// Globals
duk_context* _ctx = 0;
std::stack<iIsolate *> _isolates;



namespace v8 {

// -------------------------------------------------------------------------
// class Platform
class i4Platform : public Platform {
public:
  i4Platform() {}
  void CallOnBackgroundThread(Task* task,
                              ExpectedRuntime expected_runtime) override {
    task->Run();
    delete task;
  }
  void CallOnForegroundThread(v8::Isolate* isolate, Task* task) override {
    task->Run();
    delete task;
  }
  void CallDelayedOnForegroundThread(v8::Isolate* isolate, Task* task,
                                     double delay_in_seconds) override {
    delete task;
  }
  void CallIdleOnForegroundThread(v8::Isolate* isolate,
                                  IdleTask* task) override {}
  double MonotonicallyIncreasingTime() override {
    return synthetic_time_in_sec_ += 0.00001;
  }
  bool IdleTasksEnabled(v8::Isolate* isolate) override { return false; }
  uint64_t AddTraceEvent(char phase, const uint8_t* categoryEnabledFlag,
                         const char* name, const char* scope, uint64_t id,
                         uint64_t bind_id, int numArgs, const char** argNames,
                         const uint8_t* argTypes, const uint64_t* argValues,
                         unsigned int flags) override {
    return 0;
  }
  void UpdateTraceEventDuration(const uint8_t* categoryEnabledFlag,
                                const char* name, uint64_t handle) override {}
  const uint8_t* GetCategoryGroupEnabled(const char* name) override {
    static uint8_t no = 0;
    return &no;
  }

  const char* GetCategoryGroupName(
      const uint8_t* categoryEnabledFlag) override {
    static const char* dummy = "dummy";
    return dummy;
  }
private:
  double synthetic_time_in_sec_ = 0.0;
};

namespace platform {
  Platform* CreateDefaultPlatform(int thread_pool_size) {
    return new i4Platform();
  }
  bool PumpMessageLoop(v8::Platform* platform, v8::Isolate* isolate) {
    return true;
  }
}// namespace platform

// -------------------------------------------------------------------------
// class ResourceConstraints
ResourceConstraints::ResourceConstraints() {}
void ResourceConstraints::ConfigureDefaults(uint64_t physical_memory,
                         uint64_t virtual_memory_limit) {}


typedef std::stack<HandleScope*> HandleScopeStack;
HandleScopeStack _hscopes;

typedef std::multimap<HandleScope*,iBase*> HandleScopeMap;
HandleScopeMap _handles;

void TrackHandle(iBase* handle) {
  if (_hscopes.size()) {
    HandleScope* hs = _hscopes.top();
    _handles.insert(std::pair<HandleScope*, iBase*>(hs, handle));
  }
}

// -------------------------------------------------------------------------
// class HandleScope
HandleScope::HandleScope(Isolate* isolate) {
  I4T;
  // Start to track all stack-allocated objects associated with
  // this isolate.
  _hscopes.push(this);
}
HandleScope::~HandleScope() {
  I4T;
  _hscopes.pop();
  
  // delete anything allocated since the ctor
  for (HandleScopeMap::iterator it=_handles.find(this);
      it != _handles.end();)
  {
    delete it->second;
    it = _handles.erase(it);
  }
}
void HandleScope::Initialize(Isolate* isolate) {
  I4T;
}

// -------------------------------------------------------------------------
// class Context

/** 
  * v8 Context -> duktape context
  *  create a new context inside a given heap (isolate)
  *
  *     duk_push_thread(ctx);
  *     duk_context* new_ctx = duk_get_context(ctx, -1);
  *     
  *
  *
  * v8 isolate -> duktape heap (no shared globals)
  *     ctx = duk_create_heap_default();
  *
  */


/* static */ Local<Context> Context::New(
      Isolate* isolate, ExtensionConfiguration* extensions,
      Local<ObjectTemplate> global_template,
      Local<Value> global_object)
{
  I4T;

  // create a new context in the "top" isolate passed in!
  iIsolate* i = reinterpret_cast<iIsolate*>(isolate);
  iContext* c = new iContext();

  // Track with the handlescope
  TrackHandle(c);

  (void)duk_push_thread(i->ctx);
  c->ctx = duk_get_context(i->ctx, -1);
  return Local<Context>(Local<Context>(reinterpret_cast<Context*>(c)));
}
void Context::Enter() {
  I4T;
}
void Context::Exit() {
  I4T;
}
Isolate* Context::GetIsolate() {
  I4T;
  return 0;
}


// -------------------------------------------------------------------------
// class Value
bool Value::IsTrue() const { return false; }
Maybe<int32_t> Value::Int32Value(
      Local<Context> context) const {
  I4T;
  return Just<int32_t>(0);
}

// -------------------------------------------------------------------------
// class String
/* static */MaybeLocal<String> String::NewFromUtf8(
    Isolate* isolate, const char* data, v8::NewStringType type,
    int length)
{
  I4T;
  // V8 would put this into the heap, but we won't commit it to 
  // the duktape heap until we know what it's being used for.
  iString* str = new iString();
  str->s = data;
  String* v8_str = reinterpret_cast<String*>(str);
  return Local<String>(Local<String>(v8_str));
}
int String::Length() const { I4T; return 0; }
int String::Utf8Length() const { I4T; return 0; }
bool String::IsOneByte() const { I4T; return false; }
bool String::ContainsOnlyOneByte() const { I4T; return false; }


// -------------------------------------------------------------------------
// class String::Utf8value
String::Utf8Value::Utf8Value(Local<v8::Value> obj) {
  I4T;
}
String::Utf8Value::~Utf8Value() {
  I4T;
}

// -------------------------------------------------------------------------
// class Script
/*static */ MaybeLocal<Script> Script::Compile(
      Local<Context> context, Local<String> source,
      ScriptOrigin* origin)
{
  I4T;
  return MaybeLocal<Script>();
}
/* static */ MaybeLocal<Value> Script::Run(Local<Context> context) {
  I4T;
  return MaybeLocal<Value>();
}


// -------------------------------------------------------------------------
// class Message
Local<String> Message::Get() const {
  I4T;
  return Local<String>();
}
MaybeLocal<String> Message::GetSourceLine(
      Local<Context> context) const {
  I4T;
  return MaybeLocal<String>();
}
ScriptOrigin Message::GetScriptOrigin() const {
  I4T;
  return ScriptOrigin(Local<String>());
}
Local<Value> Message::GetScriptResourceName() const {
  I4T;
  return Local<Value>();
}
Local<StackTrace> Message::GetStackTrace() const {
  I4T;
  return Local<StackTrace>();
}
Maybe<int> Message::GetLineNumber(Local<Context> context) const {
  I4T;
  return Just<int>(0);
}
int Message::GetStartPosition() const {
  I4T;
  return 0;
}
int Message::GetEndPosition() const {
  I4T;
  return 0;
}
Maybe<int> Message::GetStartColumn(Local<Context> context) const {
  I4T;
  return Just<int>(0);
}
Maybe<int> Message::GetEndColumn(Local<Context> context) const {
  I4T;
  return Just<int>(0);
}

// -------------------------------------------------------------------------
// class V8
void V8::SetNativesDataBlob(StartupData* startup_blob) {I4T;}
void V8::SetSnapshotDataBlob(StartupData* startup_blob) {I4T;}
StartupData V8::CreateSnapshotDataBlob(const char* custom_source) {
  I4T;
  return StartupData{0,0};
}
void V8::InitializePlatform(Platform* platform) {I4T;}
void V8::SetFlagsFromString(const char* str, int length) {I4T;}
void V8::SetFlagsFromCommandLine(int* argc,
                              char** argv,
                              bool remove_flags) {I4T;}
bool V8::Initialize() {
  I4T;
  _ctx = duk_create_heap_default();
  return true;
}
bool V8::InitializeICU(const char* icu_data_file) {
  I4T;
  return true;
}
bool V8::Dispose() {
  I4T;
  duk_destroy_heap(_ctx);
  _ctx = 0;
  return true;
}
const char* V8::GetVersion() {
  return "i4 JavaScript Engine";
}
void V8::ToLocalEmpty() {I4T;}
void V8::FromJustIsNothing() {I4T;}
void V8::ShutdownPlatform() {I4T;}
void V8::InitializeExternalStartupData(const char* directory_path) {I4T;}
void V8::InitializeExternalStartupData(const char* natives_blob,
                                            const char* snapshot_blob) {I4T;}


// -------------------------------------------------------------------------
// class Template
void Template::Set(Local<Name> name, Local<Data> value,
           PropertyAttribute attributes) {
  I4T;
}



// -------------------------------------------------------------------------
// class ObjectTemplate
/* static */ Local<ObjectTemplate> ObjectTemplate::New(
      Isolate* isolate,
      Local<FunctionTemplate> constructor) {
  I4T;
  // api.cc : return New(reinterpret_cast<i::Isolate*>(isolate), constructor);


  iIsolate* i = reinterpret_cast<iIsolate*>(isolate);
  iObjectTemplate* ot = new iObjectTemplate();
  iFunctionTemplate* ft = 0;

  if (!constructor.IsEmpty()) {
    ft = new iFunctionTemplate();
    assert(0);
  }

  ObjectTemplate* v8_ot = reinterpret_cast<ObjectTemplate*>(ot);

  return Local<ObjectTemplate>();
}

/**
 * in V8's impl, nearly all objects are stored in the heap itself, and
 * utility classes like Local<> are used to protect data from being
 * collected while they need to be kept in memory. 
 *
 * in i4, we don't store everything in the heap; however the lifetime
 * is still important as the v8.h client expects certain behavior
 *
 */


// -------------------------------------------------------------------------
// class FunctionTemplate
/* static */ Local<FunctionTemplate> FunctionTemplate::New(
      Isolate* isolate, FunctionCallback callback,
      Local<Value> data,
      Local<Signature> signature, int length) {
  I4T;
  iFunctionTemplate* ft = new iFunctionTemplate();
  // TODO store signature
  FunctionTemplate* v8_ft =  reinterpret_cast<FunctionTemplate*>(ft);
  
  //return Local<FunctionTemplate>(Local<FunctionTemplate>(v8_ft));
  return Local<FunctionTemplate>();
}

// -------------------------------------------------------------------------
// class TryCatch
TryCatch::TryCatch(Isolate* isolate) {
  I4T;
}
TryCatch::~TryCatch() {I4T;}
MaybeLocal<Value> TryCatch::StackTrace(Local<Context> context) const {
  I4T;
  return Local<Value>();
}
Local<v8::Message> TryCatch::Message() const {
  I4T;
  return Local<v8::Message>();
}
void TryCatch::Reset() {I4T;}
void TryCatch::SetVerbose(bool) {I4T;}
void TryCatch::SetCaptureMessage(bool) {I4T;}
bool TryCatch::HasCaught() const { I4T; return false; }
bool TryCatch::CanContinue() const { I4T; return true; }
bool TryCatch::HasTerminated() const { I4T; return false; }
Local<Value> TryCatch::ReThrow() {
  I4T;
  return Local<Value>();
}
Local<Value> TryCatch::Exception() const {
  I4T;
  return Local<Value>();
}

// -------------------------------------------------------------------------
// class Isolate
/* static */ Isolate* Isolate::New(const CreateParams& params) {
  I4T;
  iIsolate* i = new iIsolate();
  Isolate* v8_isolate = reinterpret_cast<Isolate*>(i);

  // Create a new duktape heap for this. Note this heap does not share
  // globals with anything else. Code won't actually be run in this
  // context; the caller will create new v8::contexts for this.
  i->ctx = duk_create_heap_default();
  return v8_isolate;
}
/* static */ Isolate* Isolate::GetCurrent() {
  I4T;
  return 0;
}
void Isolate::Enter() {
  I4T;
  iIsolate* i = reinterpret_cast<iIsolate*>(this);
  _isolates.push(i);
}
void Isolate::Exit() {
  I4T;
  iIsolate* i = reinterpret_cast<iIsolate*>(this);
  _isolates.pop();
}
void Isolate::Dispose() {
  I4T;
  iIsolate* i = reinterpret_cast<iIsolate*>(this);
  duk_destroy_heap(i->ctx);
  delete i;
}
void Isolate::DiscardThreadSpecificMetadata() {I4T;}
size_t Isolate::NumberOfHeapSpaces() {
  I4T;
  return 0;
}
bool Isolate::InContext() {
  I4T;
  iIsolate* i = reinterpret_cast<iIsolate*>(this);
  return true;
}
Local<Value> Isolate::ThrowException(Local<Value> exception) {
  I4T;
  return Local<Value>();
}
Local<Context> Isolate::GetCurrentContext() {
  I4T;
  return Local<Context>();
}
bool Isolate::GetHeapSpaceStatistics(HeapSpaceStatistics* space_statistics,
                              size_t index) { I4T; return 0; }
size_t Isolate::NumberOfTrackedHeapObjectTypes() {
  I4T;
  return 0;
}

} //  namespace v8
