extern BOOL
InlineHook(
		   IN    void *pfnOrgProc,            /* 需要Hook的函数地址 */
		   IN    void *pfnNewProc,        /* 代替被Hook函数的地址 */
		   OUT    void **pfnRealProc        /* 返回原始函数的入口地址 */
		   );
extern void UnInlineHook(
				  void *pfnOrgProc,        /* 需要恢复Hook的函数地址 */
				  void *pfnRealProc    /* 原始函数的入口地址 */
				  );