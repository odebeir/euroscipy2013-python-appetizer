# -*- coding: utf-8 -*-
# Notebook tools

from IPython.core.display import HTML
import urllib

def disp_code(c,h=500,w=1024):
    """Display an interactive Python Tuto iframe the the given code
    c = source code to test (string)
    """
    fmt = """<iframe width="%(width)d" height="%(height)d" frameborder="0"\
src="http://pythontutor.com/iframe-embed.html#\
code=%(code)s\
&cumulative=%(cumulative)s\
&heapPrimitives=%(heap)s\
&drawParentPointers=%(pointer)s\
&textReferences=%(textref)s\
&showOnlyOutputs=%(output_only)s\
&py=%(py)d\
&curInstr=%(curinst)d"> </iframe>"""
    d = {"cumulative":False,"heap":False,"pointer":False,"textref":False,"output_only":False,"py":2,"curinst":0,"height":h,"width":w}
    d["code"] = urllib.urlencode({"c":c.strip('\n')})[2:] #URL encode the code string (after removing leading \n)
    return HTML(fmt%d)



