#! /usr/bin/env python
#  This a program which convert vcproj(vc project file) to makefile(linux makefile file)
#  This is edited by houzhifang in 2010.01.06

import fnmatch
import os
import os.path
import re
import sys
import xmllib

class VCProjParser(xmllib.XMLParser):
	def __init__(self, parent):
		xmllib.XMLParser.__init__(self)
		self.parent = parent
		self.config = ''
		
	def handle_starttag(self, tag, method, attributes):
		print tag
		
	def unknown_starttag(self, tag, attributes):
		if tag == 'File':
			for x in attributes.items():
				if x[0] == 'RelativePath':
					self.parent.addfile(x[1])
		elif tag == 'VisualStudioProject':
			for x in attributes.items():
				if x[0] == 'Name':
					self.parent.setprojname(x[1])
		elif tag == 'Tool':
			if self.parent.config != 'Debug|Win32':
				return
			for x in attributes.items():
				if x[0] == 'Name' and x[1] == 'VCLibrarianTool':
					self.parent.style = 'lib'
				elif x[0] == 'Name' and x[1] == 'VCLinkerTool':
					self.parent.style = 'exe'
				elif x[0] == 'AdditionalIncludeDirectories':
					self.parent.includes = self.incdir2list(x[1])
				elif x[0] == 'PreprocessorDefinitions':
					self.parent.defines = self.def2list(x[1])
		elif tag == 'Configuration':
			for x in attributes.items():
				if x[0] == 'Name':
					self.parent.config = x[1]
					
	def unknown_endtag(self, tag):
		if tag == 'Configuration':
			self.parent.config = ''
			
	def incdir2list(self, incdir):
		incdir = incdir.replace('\\', '/')
		return incdir.split(';')

		
	def def2list(self, define):
		define = define.replace('XAIT_PLATFORM_WIN32;', 'XAIT_PLATFORM_LINUX32;').replace('WIN32;', '').replace('WIN32', '').replace('__WINDOWS;', '').replace('__WINDOWS', '').replace(';;', ';')
                if define[len(define)-1] == ';':
                   define = define + '__LINUX'
                else:
                   define = define + ';__LINUX'
		return define.split(';')
		
	def pathnormal(self, path):
		return path.replace('\\', '/')

class project:
	allowfiles = ['*.c', '*.cpp', '*.cc', '*.h', '*.hpp']
	def __init__(self, style = 'exe'):
		self.parser = VCProjParser(self)
		self.projname = ''
		self.files = []
		self.includes = []
		self.defines = []
		self.style = style
		self.inc = ''
		self.basedir = ''
		self.amre = '^(lib)?%s_(.+)\s*=(.+)$'
		self.amset = {}
		self.amset1 = {}
		
	def makeuniqname(self):
		if self.style == 'exe':
			return '%s'%self.projname
		if self.style == 'lib':
			return 'lib%s_a'%self.projname
			
	def loadvcproj(self, vcproj):
		self.parser.reset()
		f = open(vcproj, 'r')
		self.parser.feed(f.read())
		self.parser.close()
		f.close()
		self.amset['SOURCES'] = ' '.join(self.files)
		self.amset['DEFINES'] = ' '.join(['-D' + l for l in self.defines])
		self.amset['INCLUDES']= ' '.join(['-I' + l for l in self.includes])
		self.amset1['CFLAGS'] = '$(%s_DEFINES) $(%s_INCLUDES)'%(self.makeuniqname(), self.makeuniqname())
		self.amset1['CXXFLAGS'] = '$(%s_DEFINES) $(%s_INCLUDES)'%(self.makeuniqname(), self.makeuniqname())
		if self.style != 'lib':
			self.amset['LDADD'] = '$(ALLLIBS)'
			
	def mergemakeam(self, amfile):
		mretext = self.amre%self.makeuniqname()
		myre = re.compile(mretext)
		try:
			f = open(amfile, 'r')
			for l in f:
				m = myre.match(l)
				if m:
					k = m.group(2).replace(' ','')
					v = m.group(3)
					if k == 'SOURCES': continue
					if k == 'CXXFLAGS' or k == 'CFLAGS':
						self.amset1[k] = v
					else:
						self.amset[i] = v
			f.close()
		except:
			pass
				
	def addfile(self, file):
		file = file.replace('\\', '/')
		for a in project.allowfiles:
			if fnmatch.fnmatch(file, a):
				self.files.append(file)
				return
				
	def setprojname(self, name):
		self.projname = name.split('.')[-1]
		
	def output(self, outf):
		for k in self.amset.keys():
			outf.write('%s_%s=%s\n'%(self.makeuniqname(), k, self.amset[k]))
		for k in self.amset1.keys():
			outf.write('%s_%s=%s\n'%(self.makeuniqname(), k, self.amset1[k]))
		outf.write('\n')
  	 	

class makefile:
	def __init__(self, path):
		self.ldadd = ''
		self.text  = ''
		self.count = 0
		self.progs = []
		self.libs  = []
		if path == '':
			self.makefile = 'Makefile.am'
			self.path = '.'
		else:
			self.makefile = path + '/Makefile.am'
			self.path = path
	
	def findproj(self):
		for fn in os.listdir(self.path):
			if fnmatch.fnmatch(fn, '*.vcproj'):
#				now begin init the vcproj file 				
				print fn
				p = project()
				p.loadvcproj(self.path+'/'+fn)
				p.mergemakeam(self.makefile)
				p.output(self)
				if p.style == 'lib':
					self.libs.append(p.makeuniqname().replace('_', '.'))
				elif p.style == 'exe':
					self.progs.append(p.makeuniqname())
				self.count = self.count + 1
	
	def write(self, text):
		self.text = self.text + text
		
	def output(self):
		if self.count == 0:
			print 'Error'
			return
		outf = open(self.makefile, 'w')
#		outf = sys.stdout
#		outf.write('include $(top_srcdir) Makefile.common.in\n')
		if len(self.progs) > 0:
			outf.write('bin_PROGRAMS=%s\n'%' '.join(self.progs))
		if len(self.libs) > 0:
			outf.write('lib_LIBRARIES=%s\n'%' '.join(self.libs))
		outf.write(self.text)
		outf.close()



if len(sys.argv) == 1:
   path='.'
else:
   path=sys.argv[1]

m = makefile(path)
m.findproj()
m.output()
