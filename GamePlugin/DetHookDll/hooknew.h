extern BOOL
InlineHook(
		   IN    void *pfnOrgProc,            /* ��ҪHook�ĺ�����ַ */
		   IN    void *pfnNewProc,        /* ���汻Hook�����ĵ�ַ */
		   OUT    void **pfnRealProc        /* ����ԭʼ��������ڵ�ַ */
		   );
extern void UnInlineHook(
				  void *pfnOrgProc,        /* ��Ҫ�ָ�Hook�ĺ�����ַ */
				  void *pfnRealProc    /* ԭʼ��������ڵ�ַ */
				  );