

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "duktape.h"

void test_function_binding(const char* jsfile);
void test_weak_refs(const char* jsfile);
int dump_value_stack(duk_context *, const char* msg = 0);


class Foo {
public:
	Foo() {
		//printf("Foo()\n");
	}
	~Foo() {
		//printf("~Foo()\n");
	}
	void bar(const char*) {
		//printf("native bar()\n");
	}
};

duk_ret_t Foo_dtor(duk_context *ctx)
{
	printf("js_Foo_dtor\n");

  // The object to delete is passed as first argument instead
  duk_get_prop_string(ctx, 0, "\xff""\xff""deleted");

  bool deleted = duk_to_boolean(ctx, -1);
  duk_pop(ctx);

  if (!deleted) {
      duk_get_prop_string(ctx, 0, "\xff""\xff""data");
      delete static_cast<Foo *>(duk_to_pointer(ctx, -1));
      duk_pop(ctx);

      // Mark as deleted
      duk_push_boolean(ctx, true);
      duk_put_prop_string(ctx, 0, "\xff""\xff""deleted");
  }

  return 0;
}

extern "C" duk_ret_t Foo_bar(duk_context* ctx) {

	duk_size_t sz = 0;
	const char* arg = duk_require_lstring(ctx, 0, &sz);

	printf("Foo::bar(%s)\n", arg ? arg : "<none>");
	return 1;
}

extern "C" duk_ret_t Foo_ctor(duk_context *ctx) {
	printf("js_Foo_ctor\n");

  // Get arguments
  //float x = duk_require_number(ctx, 0);
  //float y = duk_require_number(ctx, 1);

  // Push special this binding to the function being constructed
  duk_push_this(ctx);

  // Store the underlying object
  duk_push_pointer(ctx, new Foo());
  duk_put_prop_string(ctx, -2, "\xff""\xff""data");

  // Store a boolean flag to mark the object as deleted because the destructor may be called several times
  //duk_push_boolean(ctx, false);
  //duk_put_prop_string(ctx, -2, "\xff""\xff""deleted");

  // Store the function destructor
  duk_push_c_function(ctx, Foo_dtor, 1);
  duk_set_finalizer(ctx, -2);

  return 1;
}


// methods, add more here
const duk_function_list_entry Foo_methods[] = {
    { "bar",   Foo_bar,  1   },
    { 0,  0,        0   }
};


int dump_value_stack(duk_context *c, const char* msg)
{
  int i=-1;
  int count = 0;
  const char* s;

  printf("---%s-------------------------\n", msg?msg:"");

  while(1) {
    int type = duk_get_type(c, i);
    if (DUK_TYPE_NONE == type)
      break;

    count++;

    printf("%d ", i);

    switch(type) {
      case DUK_TYPE_UNDEFINED: 
        printf("DUK_TYPE_UNDEFINED\n");
        break;
      case DUK_TYPE_NULL: 
        printf("DUK_TYPE_NULL\n");
        break;
      case DUK_TYPE_BOOLEAN: 
        printf("DUK_TYPE_BOOLEAN\n");
        break;
      case DUK_TYPE_NUMBER: 
        printf("DUK_TYPE_NUMBER\n");
        break;
      case DUK_TYPE_OBJECT: 
        printf("DUK_TYPE_OBJECT\n");
        break;
      case DUK_TYPE_BUFFER: 
        printf("DUK_TYPE_BUFFER\n");
        break;
      case DUK_TYPE_POINTER: 
        printf("DUK_TYPE_POINTER\n");
        break;
      case DUK_TYPE_STRING: 
        printf("DUK_TYPE_POINTER\n");
        break;
      case DUK_TYPE_LIGHTFUNC: 
        printf("DUK_TYPE_LIGHTFUNC\n");
        break;
      default:
        printf("Unknown type=%d\n", type);
    }
    
    //s = duk_safe_to_string(c, i); 
    //printf("     %s\n",s?s:"");
    i--;
  }

  printf("\n");
  return count;
}


duk_context *ctx = 0;


int main(int argc, char** argv)
{
  const char* file_to_test = "test_func.js";
  if (argc > 1)
    file_to_test = argv[1];

  if( (ctx = duk_create_heap_default()) ) {
    //test_function_binding(file_to_test);
    test_weak_refs(file_to_test);
  }

  duk_destroy_heap(ctx);

  return 0;
}


duk_ret_t my_weak_ref_callback(duk_context *ctx) {
  // Object being finalized is at stack index 0.
  void* ptr = duk_get_heapptr(ctx, -1); // or -1?

  printf("we are holding the last handle to this object. ptr=%p\n", ptr);
  return 0;
}

void test_weak_refs(const char* )
{
  /*
  duk_push_c_function(ctx, Foo_ctor, 0);
  duk_push_object(ctx);
  duk_put_function_list(ctx, -1, Foo_methods);
  duk_put_prop_string(ctx, -2, "prototype");
  duk_put_global_string(ctx, "Foo");
  */

  const char* kTestString = "var f = {}; f\n";

  dump_value_stack(ctx, "init");
  //duk_eval_string(ctx, kTestString);
  //dump_value_stack(ctx, "after eval");

  // grab a reference to the return value, then gc()
  //if (duk_is_object(ctx, -1))
  {
    // push an object to the stack.
    duk_idx_t obj_idx;

    obj_idx = duk_push_object(ctx);
    duk_push_int(ctx, 42);
    duk_put_prop_string(ctx, obj_idx, "meaningOfLife");

    // add a finalizer to this object.
    duk_push_c_function(ctx, my_weak_ref_callback, 1);
    //duk_set_finalizer(ctx, -2);
    duk_set_weakref(ctx, -2);
    void* ptr = duk_get_heapptr(ctx, -1);
    printf("ptr = %p\n", ptr);
    dump_value_stack(ctx, "stack should have one value");

    // pop the result from the stack.
    duk_pop(ctx);
    dump_value_stack(ctx, "stack be zero now");

    // force a collection, forcing a weak-ref to be triggered.
    dump_value_stack(ctx, "before gc");
    duk_gc(ctx, 0);
    duk_gc(ctx, 0);
    dump_value_stack(ctx, "after gc");
  }
  //else
    //printf("Error: %s\n", duk_safe_to_string(ctx, -1));
}

void test_function_binding(const char* jsfile)
{
  // push a funcation callback into the value stack as
  // an ECMAScript function
  //dump_value_stack(ctx);

	duk_push_c_function(ctx, Foo_ctor, /*nargs=*/0);
  //printf("duk_push_c_function(ctx, Foo_ctor, /*nargs=*/0);\n");
  //dump_value_stack(ctx);

  // push empty object onto the value stack (at index -1)
	duk_push_object(ctx);
  //printf("duk_push_object(ctx);\n");
  //dump_value_stack(ctx);

  // [Foo_ctor] [null]

  // replace empty object in value stack with our list of methods
	duk_put_function_list(ctx, -1, Foo_methods);
  //printf("duk_put_function_list(ctx, -1, Foo_methods);\n");
  //dump_value_stack(ctx);
  // [Foo_ctor] [Foo_methods]

  // into the stack position -1 (below the methods) place a name
	duk_put_prop_string(ctx, -2, "prototype");
  //printf("duk_put_prop_string(ctx, -2, \"prototype\");\n");
  //dump_value_stack(ctx);
  // [Foo_ctor]

	// now store as a global
  //printf("duk_put_global_string(ctx, \"Foo\");\n");
	duk_put_global_string(ctx, "Foo");
  //dump_value_stack(ctx);

	if (!duk_peval_file(ctx, jsfile))
	{
		duk_pop(ctx); // ignore the result from the eval.

    duk_gc(ctx, 0);
	}
	else
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));

  //dump_value_stack(ctx);
}

