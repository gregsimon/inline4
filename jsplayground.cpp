

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "duktape.h"

class Foo {
public:
	Foo() {
		printf("Foo()\n");
	}
	~Foo() {
		printf("~Foo()\n");
	}
	void bar(const char*) {
		printf("native bar()\n");
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


void dump_value_stack(duk_context *c)
{
  int i=-1;
  const char* s;

  while(1) {
    int type = duk_get_type(c, i);
    if (DUK_TYPE_NONE == type)
      break;

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
}



int main(int argc, char** argv)
{
	duk_context *ctx = 0;
	

	

	if( (ctx = duk_create_heap_default()) )
	{
    // push a funcation callback into the value stack as 
    // an ECMAScript function
    dump_value_stack(ctx);

		duk_push_c_function(ctx, Foo_ctor, /*nargs=*/0);
    printf("duk_push_c_function(ctx, Foo_ctor, /*nargs=*/0);\n");
    dump_value_stack(ctx);

    // push empty object onto the value stack (at index -1)
		duk_push_object(ctx);
    printf("duk_push_object(ctx);\n");
    dump_value_stack(ctx);

    // [Foo_ctor] [null]

    // replace empty object in value stack with our list of methods
		duk_put_function_list(ctx, -1, Foo_methods);
    printf("duk_put_function_list(ctx, -1, Foo_methods);\n");
    dump_value_stack(ctx);
    // [Foo_ctor] [Foo_methods]

    // into the stack position -1 (below the methods) place a name
		duk_put_prop_string(ctx, -2, "prototype");
    printf("duk_put_prop_string(ctx, -2, \"prototype\");\n");
    dump_value_stack(ctx);
    // [Foo_ctor] 
		
		// now store as a global
    printf("duk_put_global_string(ctx, \"Foo\");\n");
		duk_put_global_string(ctx, "Foo");
    dump_value_stack(ctx);


		if (!duk_peval_file(ctx, "test_func.js"))
		{
			duk_pop(ctx); // ignore the result from the eval.

			
		}
		else
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));

    dump_value_stack(ctx);

	}

	duk_destroy_heap(ctx);
	return 0;
}

