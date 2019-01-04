#ifndef __ERROR_INC
#define __ERROR_INC

/* Error Data */
typ ErrorDataT: record
(
   FileName: @StringT,
   Line    : @StringT,
   LineSize: Int32T,
   LineNo  : Int32T,
   LineOff : Int32T
);

/* Export error flag */
dec ErrorFlag : BooleanT;

/* Interface functions */
dec ErrorTrigger: func();
dec ErrorHeader : func(ErrorData: @ErrorDataT);
dec ErrorBodyStr: func(String   : @StringT   );
dec ErrorBodyInt: func(Integer  : Int32T     );
dec ErrorFooter : func(ErrorData: @ErrorDataT);

/* Init functions */
dec ErrorInit   : func();
dec ErrorCleanup: func();

#endif /* __ERROR_INC */
