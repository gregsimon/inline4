

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "duktape.h"

extern "C" duk_ret_t Foo_bar(duk_context* ctx) {
	printf("Foo::bar()\n");
	return 1;
}

extern "C" duk_ret_t Foo_constructor(duk_context *ctx) {
	printf("NATIVE FOO CONSTRUCTED\n");

	// Add the methods, props to this object.

	duk_push_c_function(ctx, Foo_bar, 1);
	duk_put_prop_string(ctx, -2, "bar");
	duk_pop(ctx);

    duk_push_int(ctx, 123);
    return 1;
}




int main(int argc, char** argv)
{
	duk_context *ctx = 0;
	

	

	if( (ctx = duk_create_heap_default()) )
	{
		// set up the VM with our custom bindings.
		duk_push_global_object(ctx);
	    duk_push_c_function(ctx, Foo_constructor, 0 /*nargs*/);
	    duk_put_prop_string(ctx, -2, "Foo");
	    duk_pop(ctx);


		if (!duk_peval_file(ctx, "test_func.js"))
		{
			duk_pop(ctx); // ignore the result from the eval.


			
		}
		else
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));



	}

	duk_destroy_heap(ctx);
	return 0;
}

