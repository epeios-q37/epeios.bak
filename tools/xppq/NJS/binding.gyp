{
 "conditions": [
  [
   "OS=='win'",
   {
    "variables": {
     "epeios": "<!(IF DEFINED EPEIOS_SRC (echo ../../../stable/) ELSE (echo src/epeios/))",
     "src": "<!(IF DEFINED EPEIOS_SRC (echo ./) ELSE (echo src/) )",
    },
   },
   {
    "variables": {
     "epeios": "<!(if [ \"$EPEIOS_SRC\" != \"\" ]; then echo ../../../stable/; else echo src/epeios/; fi)",
     "src": "<!(if [ \"$EPEIOS_SRC\" != \"\" ]; then echo ./; else echo src/; fi)",
    },
   }
  ]
 ],
 "targets": [
  {
   "target_name": "xppqnjs",
   "sources": [ "xppqnjs.cpp", "<(src)/parser.cpp", "<(src)/stream.cpp", "<(src)/common.cpp", "<(epeios)/ags.cpp", "<(epeios)/aem.cpp", "<(epeios)/bch.cpp", "<(epeios)/bitbch.cpp", "<(epeios)/bso.cpp", "<(epeios)/cio.cpp", "<(epeios)/cpe.cpp", "<(epeios)/crptgr.cpp", "<(epeios)/cslio.cpp", "<(epeios)/crt.cpp", "<(epeios)/ctn.cpp", "<(epeios)/dir.cpp", "<(epeios)/dte.cpp", "<(epeios)/dtfbsc.cpp", "<(epeios)/dtfptb.cpp", "<(epeios)/epsmsc.cpp", "<(epeios)/err.cpp", "<(epeios)/fdr.cpp", "<(epeios)/flf.cpp", "<(epeios)/flsq.cpp", "<(epeios)/flw.cpp", "<(epeios)/flx.cpp", "<(epeios)/ias.cpp", "<(epeios)/idsq.cpp", "<(epeios)/iof.cpp", "<(epeios)/iop.cpp", "<(epeios)/lck.cpp", "<(epeios)/lst.cpp", "<(epeios)/lstbch.cpp", "<(epeios)/lstcrt.cpp", "<(epeios)/lstctn.cpp", "<(epeios)/mns.cpp", "<(epeios)/mtk.cpp", "<(epeios)/mtx.cpp", "<(epeios)/ntvstr.cpp", "<(epeios)/que.cpp", "<(epeios)/sdr.cpp", "<(epeios)/stkbse.cpp", "<(epeios)/stkbch.cpp", "<(epeios)/stkctn.cpp", "<(epeios)/str.cpp", "<(epeios)/strng.cpp", "<(epeios)/tagsbs.cpp", "<(epeios)/tol.cpp", "<(epeios)/txf.cpp", "<(epeios)/tys.cpp", "<(epeios)/uys.cpp", "<(epeios)/utf.cpp", "<(epeios)/xtf.cpp", "<(epeios)/llio.cpp", "<(epeios)/bomhdl.cpp", "<(epeios)/cdgb64.cpp", "<(epeios)/fil.cpp", "<(epeios)/fnm.cpp", "<(epeios)/lcl.cpp", "<(epeios)/ntvstr.cpp", "<(epeios)/rgstry.cpp", "<(epeios)/stsfsm.cpp", "<(epeios)/xml.cpp", "<(epeios)/xpp.cpp", "<(epeios)/tht.cpp", "<(epeios)/uvq.cpp", "<(epeios)/v8q.cpp", "<(epeios)/v8qnjs.cpp", "<(epeios)/scln.cpp", 
   ],
   "include_dirs":  [ "<(epeios)" ],
   "conditions": [
    [
     "OS=='win'",
     {
      "sources": ["<(epeios)/wllio.cpp",       ],
      "msvs_settings": {
       "VCCLCompilerTool": {
        "ExceptionHandling": 1
       }
      },
     },
     {
      "sources": ["<(epeios)/pllio.cpp",       ],
      "cflags_cc": ["-std=gnu++11", "-fexceptions"],
     }
    ],
    [
     "OS=='mac'",
     {
      "cflags_cc": ["-DMTX_NATIVE"],
     }
    ],
   ],
  },
 ],
}
