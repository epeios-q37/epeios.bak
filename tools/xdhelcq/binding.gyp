{
 "variables": {
  "stable": "../../stable/",
  "ext": "node",
 },
 "conditions": [
  [
   "OS=='win'",
   {
    "variables": {
     "epeios": "<!(IF DEFINED Q37_EPEIOS (echo <(stable)) ELSE (echo src/epeios/))",
     "src": "<!(IF DEFINED Q37_EPEIOS (echo ./) ELSE (echo src/) )",
    },
   },
   {
    "variables": {
     "epeios": "<!(if [ \"$Q37_EPEIOS\" != \"\" ]; then echo <(stable); else echo src/epeios/; fi)",
     "src": "<!(if [ \"$Q37_EPEIOS\" != \"\" ]; then echo ./; else echo src/; fi)",
    },
   }
  ]
 ],
 "targets": [
  {
   "target_name": "<(module_name)",
   "sources": [ "<(src)/xdhelcq.cpp", "<(src)/registry.cpp", "<(epeios)/ags.cpp", "<(epeios)/aem.cpp", "<(epeios)/bch.cpp", "<(epeios)/bitbch.cpp", "<(epeios)/bomhdl.cpp", "<(epeios)/bso.cpp", "<(epeios)/cdgb64.cpp", "<(epeios)/cio.cpp", "<(epeios)/cpe.cpp", "<(epeios)/crptgr.cpp", "<(epeios)/cslio.cpp", "<(epeios)/crt.cpp", "<(epeios)/ctn.cpp", "<(epeios)/dir.cpp", "<(epeios)/dte.cpp", "<(epeios)/dtfbsc.cpp", "<(epeios)/dtfptb.cpp", "<(epeios)/epsmsc.cpp", "<(epeios)/err.cpp", "<(epeios)/fdr.cpp", "<(epeios)/fil.cpp", "<(epeios)/flf.cpp", "<(epeios)/flsq.cpp", "<(epeios)/flw.cpp", "<(epeios)/flx.cpp", "<(epeios)/fnm.cpp", "<(epeios)/ias.cpp", "<(epeios)/idsq.cpp", "<(epeios)/iof.cpp", "<(epeios)/iop.cpp", "<(epeios)/lcl.cpp", "<(epeios)/lck.cpp", "<(epeios)/lst.cpp", "<(epeios)/lstbch.cpp", "<(epeios)/lstcrt.cpp", "<(epeios)/lstctn.cpp", "<(epeios)/mns.cpp", "<(epeios)/mtk.cpp", "<(epeios)/mtx.cpp", "<(epeios)/ntvstr.cpp", "<(epeios)/que.cpp", "<(epeios)/rgstry.cpp", "<(epeios)/sdr.cpp", "<(epeios)/stkbse.cpp", "<(epeios)/stkbch.cpp", "<(epeios)/stkcrt.cpp", "<(epeios)/stkctn.cpp", "<(epeios)/str.cpp", "<(epeios)/strng.cpp", "<(epeios)/stsfsm.cpp", "<(epeios)/tagsbs.cpp", "<(epeios)/tht.cpp", "<(epeios)/thtsub.cpp", "<(epeios)/tol.cpp", "<(epeios)/txf.cpp", "<(epeios)/tys.cpp", "<(epeios)/uys.cpp", "<(epeios)/utf.cpp", "<(epeios)/xml.cpp", "<(epeios)/xpp.cpp", "<(epeios)/xtf.cpp", "<(epeios)/llio.cpp", "<(epeios)/dlbrry.cpp", "<(epeios)/n4all.cpp", "<(epeios)/n4allw.cpp", "<(epeios)/n4njs.cpp", "<(epeios)/nodeq.cpp", "<(epeios)/plgn.cpp", "<(epeios)/plgncore.cpp", "<(epeios)/strmrg.cpp", "<(epeios)/uvq.cpp", "<(epeios)/uvqdcl.cpp", "<(epeios)/v8q.cpp", "<(epeios)/xdhcmn.cpp", "<(epeios)/xdhujp.cpp", "<(epeios)/xdhujr.cpp", "<(epeios)/xdhujs.cpp", "<(epeios)/xdhups.cpp", "<(epeios)/xdhutl.cpp", "<(epeios)/scla.cpp", "<(epeios)/scli.cpp", "<(epeios)/sclm.cpp", "<(epeios)/scle.cpp", "<(epeios)/scll.cpp", "<(epeios)/sclr.cpp", 
   ],
   "defines": ["VERSION=\"\"", "COPYRIGHT_YEARS=\"2017\""],
   "include_dirs":  [ "<(src)", "<(epeios)", ],
   "conditions": [
    [
     "OS=='win'",
     {
      "sources": ["<(epeios)/wllio.cpp", 
      ],
      "libraries": [
       "wsock32.lib"
      ],
      "msvs_settings": {
       "VCCLCompilerTool": {
        "RuntimeLibrary": 0,
        "ExceptionHandling": 1,
        "MultiProcessorCompilation": "Yes",
        "AdditionalOptions": [ "/EHsc", ]
       },
       "VCLinkerTool": {
       },
       "VCLibrarianTool": {
       },
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
      "defines": ["MTX_NATIVE", "MTX_SUPPRESS_WARNING"],
      "xcode_settings": { "GCC_ENABLE_CPP_EXCEPTIONS": "YES" },
     }
    ],
   ],
  },
  {
   "target_name": "action_after_build",
   "dependencies": [ "<(module_name)" ],
   "copies": [
    {
     "files": [ "<(PRODUCT_DIR)/<(module_name).<(ext)" ],
     "destination": "<(module_path)"
    }
   ]
  }
 ],
}
