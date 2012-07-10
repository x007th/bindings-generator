## ===== instance function implementation template
JSBool ${signature_name}(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
#if $is_constructor
	JSObject *obj = JS_NewObject(cx,
							 ${generator.prefix}_${class_name}_class,
							 ${generator.prefix}_${class_name}_prototype,
							 NULL); // <~ parent proto - not yet added!
#else
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	${class_name}* cobj = (${class_name} *)JS_GetInstancePrivate(cx, obj, &js_${generator.prefix}_${class_name}_class, argv);
	if (!cobj) {
		return JS_FALSE;
	}
#end if

#set arg_list = ""
#set arg_array = []
#if $min_args > 0
	#set $count = 0
	#for $arg in $arguments
	${arg} arg${count};
	#set $count = $count + 1
	#end for
	if (argc == ${min_args}) {
	#set $count = 0
	#for $arg in $arguments
		${arg.to_native($generator, "argv[" + str(count) + "]", "arg" + str(count), 2)};
		#set $arg_array += ["arg"+str(count)]
		#set $count = $count + 1
	#end for
	} else {
		return JS_FALSE;
	}
	#set $arg_list = ", ".join($arg_array)
#end if
#if $is_constructor
	cobj = new ${func_name}($arg_list);
	JS_SetPrivate(obj, cobj);
	JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
#else
	#if $ret_type.name != "void"
	${ret_type} ret = cobj->${func_name}($arg_list);
	jsval jsret;
	${ret_type.from_native($generator, "ret", "jsret", 1)};
	JS_SET_RVAL(cx, vp, jsret);
	#else
	cobj->${func_name}($arg_list);
	#end if
#end if
	return JS_TRUE;
}
