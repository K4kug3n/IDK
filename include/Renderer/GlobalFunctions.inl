

#if !defined(IDK_GLOBAL_FUNCTION)
#define IDK_GLOBAL_FUNCTION( fun )
#endif

#if !defined(IDK_GLOBAL_FUNCTION_OPTIONAL)
#define IDK_GLOBAL_FUNCTION_OPTIONAL( fun )
#endif

IDK_GLOBAL_FUNCTION(vkCreateInstance)
IDK_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)
IDK_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)
IDK_GLOBAL_FUNCTION_OPTIONAL(vkEnumerateInstanceVersion)

#undef IDK_GLOBAL_FUNCTION
#undef IDK_GLOBAL_FUNCTION_OPTIONAL