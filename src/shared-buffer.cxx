#include <nan.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

NAN_METHOD(createSharedBuffer) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  
  if (!info[0]->IsUint32()) {
    Nan::ThrowError("key must be an integer");
    return;
  } else if (!info[1]->IsUint32()) {
    Nan::ThrowError("size must be an integer");
    return;
  } 
  
  key_t key = info[0]->Uint32Value();
  size_t size = info[1]->Uint32Value();
  bool initialize = info[2]->BooleanValue();
  
  key_t shmId = shmget(key, size, initialize ? IPC_CREAT | 0666 : 0666);
  
  if (shmId < 0) {
    fprintf(stderr, "shmget failed\n");
    Nan::ThrowError(strerror(errno));
    return;
  }
    
  char * data = (char *)shmat(shmId, NULL, 0);
  
  if (data == (char *)-1) {
    fprintf(stderr, "shmat failed\n");
    Nan::ThrowError(strerror(errno));
    return;
  }
  // printf("Attached shared buffer at: %lld\n", (long long) data);
  
  if (initialize)
    memset(data, 0, size);
    
  auto buffer = Nan::New<v8::ArrayBuffer>(v8::ArrayBuffer::New(isolate, (void*)data, size));
  
  info.GetReturnValue().Set(buffer);
}

NAN_METHOD(detachSharedBuffer) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  
  // v8::Local<v8::Context> context = isolate->GetCurrentContext();
  // const bool isArrayBuffer = info[0]->InstanceOf(context, )->FromJust();
  // if (!isArrayBuffer) {
  //   Nan::ThrowError("Argument must be an ArrayBuffer");
  //   return;
  // }
  
  v8::Local<v8::ArrayBuffer> arrayBuffer = info[0].As<v8::ArrayBuffer>();
  const void *data = arrayBuffer->GetContents().Data();
  
  // printf("Detaching shared buffer at: %lld\n", (long long) data);
  int result = shmdt(data);
  
  if (result == -1) {
    Nan::ThrowError(strerror(errno));
    return;
  }
}

NAN_MODULE_INIT(Initialize) {  
  NAN_EXPORT(target, createSharedBuffer);
  NAN_EXPORT(target, detachSharedBuffer);
}

NODE_MODULE(sharedbuffer, Initialize)
