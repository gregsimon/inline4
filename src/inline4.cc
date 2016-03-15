#include "v8.h"
#include "libplatform/libplatform.h"

#include "duktape.h"

#define I4T  printf("[%s]\n",__PRETTY_FUNCTION__)

namespace v8 {

// -------------------------------------------------------------------------
// class Platform
class i4Platform : public Platform {
public:
  i4Platform() {}
  size_t NumberOfAvailableBackgroundThreads() { return 0; }
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
// class HandleScope
HandleScope::HandleScope(Isolate* isolate) {
}
HandleScope::~HandleScope() {
}

// -------------------------------------------------------------------------
// class ResourceConstraints
ResourceConstraints::ResourceConstraints() {}
void ResourceConstraints::ConfigureDefaults(uint64_t physical_memory,
                         uint64_t virtual_memory_limit) {}

// -------------------------------------------------------------------------
// class Context

/* static */ Local<Context> Context::New(
      Isolate* isolate, ExtensionConfiguration* extensions,
      Local<ObjectTemplate> global_template,
      Local<Value> global_object)
{
}
void Context::Enter() {
}
void Context::Exit() {
}
Isolate* Context::GetIsolate() {
  return 0;
}


// -------------------------------------------------------------------------
// class Value
bool Value::IsTrue() const { return false; }
/*bool Value::IsNull() const { return false; }
bool Value::IsTrue() const { return false; }
bool Value::IsFalse() const { return false; }
bool Value::IsName() const { return false; }
bool Value::IsString() const { return false; }
bool Value::IsSymbol() const { return false; }
bool Value::IsFunction() const { return false; }
bool Value::IsArray() const { return false; }
bool Value::IsObject() const { return false; }
bool Value::IsBoolean() const { return false; }
bool Value::IsNumber() const { return false; }
bool Value::IsExternal() const { return false; }
bool Value::IsInt32() const { return false; }
bool Value::IsUint32() const { return false; }
bool Value::IsDate() const { return false; }
bool Value::IsArgumentsObject() const { return false; }
bool Value::IsBooleanObject() const { return false; }
bool Value::IsNumberObject() const { return false; }
bool Value::IsStringObject() const { return false; }
bool Value::IsSymbolObject() const { return false; }
bool Value::IsNativeError() const { return false; }
bool Value::IsRegExp() const { return false; }
bool Value::IsGeneratorFunction() const { return false; }
bool Value::IsGeneratorObject() const { return false; }*/
Maybe<int32_t> Value::Int32Value(
      Local<Context> context) const {
}

// -------------------------------------------------------------------------
// class String
/* static */MaybeLocal<String> String::NewFromUtf8(
    Isolate* isolate, const char* data, v8::NewStringType type,
    int length) {

}
int String::Length() const { I4T; return 0; }
int String::Utf8Length() const { return 0; }
bool String::IsOneByte() const { return false; }
bool String::ContainsOnlyOneByte() const { return false; }


// -------------------------------------------------------------------------
// class String::Utf8value
String::Utf8Value::Utf8Value(Local<v8::Value> obj) {
}
String::Utf8Value::~Utf8Value() {
}

// -------------------------------------------------------------------------
// class Script
/*static */ MaybeLocal<Script> Script::Compile(
      Local<Context> context, Local<String> source,
      ScriptOrigin* origin) {
  I4T;
}
/* static */ MaybeLocal<Value> Script::Run(Local<Context> context) {
  I4T;
}


// -------------------------------------------------------------------------
// class Message
Local<String> Message::Get() const {
  I4T;
}
MaybeLocal<String> Message::GetSourceLine(
      Local<Context> context) const {
  I4T;
}
ScriptOrigin Message::GetScriptOrigin() const {
  I4T;
}
Local<Value> Message::GetScriptResourceName() const {
  I4T;
}
Local<StackTrace> Message::GetStackTrace() const {
  I4T;
}
Maybe<int> Message::GetLineNumber(Local<Context> context) const {
  I4T;
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
}
Maybe<int> Message::GetEndColumn(Local<Context> context) const {
  I4T;
}

// -------------------------------------------------------------------------
// class V8
void V8::SetNativesDataBlob(StartupData* startup_blob) {}
void V8::SetSnapshotDataBlob(StartupData* startup_blob) {}
StartupData V8::CreateSnapshotDataBlob(const char* custom_source) {
  return StartupData{0,0};
}
void V8::InitializePlatform(Platform* platform) {}
void V8::SetFlagsFromString(const char* str, int length) {}
void V8::SetFlagsFromCommandLine(int* argc,
                              char** argv,
                              bool remove_flags) {}
bool V8::Initialize() {
  return true;
}
bool V8::InitializeICU(const char* icu_data_file) {
  return true;
}
bool V8::Dispose() {
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
}


// -------------------------------------------------------------------------
// class FunctionTemplate
/* static */ Local<FunctionTemplate> FunctionTemplate::New(
      Isolate* isolate, FunctionCallback callback,
      Local<Value> data,
      Local<Signature> signature, int length) {
  I4T;
}

// -------------------------------------------------------------------------
// class TryCatch
TryCatch::TryCatch(Isolate* isolate) {
  I4T;
}
TryCatch::~TryCatch() {I4T;}
MaybeLocal<Value> TryCatch::StackTrace(Local<Context> context) const {
  I4T;
}
Local<v8::Message> TryCatch::Message() const {
  I4T;
}
void TryCatch::Reset() {I4T;}
void TryCatch::SetVerbose(bool) {I4T;}
void TryCatch::SetCaptureMessage(bool) {I4T;}
bool TryCatch::HasCaught() const { I4T; return false; }
bool TryCatch::CanContinue() const { I4T; return true; }
bool TryCatch::HasTerminated() const { I4T; return false; }
Local<Value> TryCatch::ReThrow() {
  I4T;
}
Local<Value> TryCatch::Exception() const {
  I4T;
}

// -------------------------------------------------------------------------
// class Isolate
/* static */ Isolate* Isolate::New(const CreateParams& params) {
I4T;
}
/* static */ Isolate* Isolate::GetCurrent() {
  I4T;
  return 0;
}
void Isolate::Enter() {
  I4T;
}
void Isolate::Exit() {
  I4T;
}
void Isolate::Dispose() {
  I4T;
}
void Isolate::DiscardThreadSpecificMetadata() {I4T;}
size_t Isolate::NumberOfHeapSpaces() {
  I4T;
  return 0;
}
bool Isolate::InContext() {
  I4T;
  return true;
}
Local<Value> Isolate::ThrowException(Local<Value> exception) {
  I4T;
}
Local<Context> Isolate::GetCurrentContext() {
  I4T;
}
bool Isolate::GetHeapSpaceStatistics(HeapSpaceStatistics* space_statistics,
                              size_t index) { I4T; return 0; }
size_t Isolate::NumberOfTrackedHeapObjectTypes() {I4T;}

} //  namespace v8
