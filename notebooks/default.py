#if this is the presenter notebook set True
#this computer should then be runing the clicker_server
#if clicker_nb_tools is not present, switch to non presenter mode

presenter_version = False

try:
    from clicker_nb_tools import ask
except ImportError:
    presenter_version = False
    
if not presenter_version:
    #replace with a dummy ask() function
    print 'you are in -listener- mode'
        
    def ask(*args,**kwargs):
        return None
else:
    print 'you are in -presenter- mode'

    
from python_tutor_nb_tools import disp_code


