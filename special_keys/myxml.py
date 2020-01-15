'''
Created on Oct 27, 2014

@author: bjoo
'''

class MyXML:
    def __init__(self):
        self.indents=[]
        self.indentstr = ''.join(self.indents)
        self.tagnames=[]
        
    def writeHeader(self):
        print "<?xml version=\"1.0\"?>"
        
    def writeTag(self,tagname,tagvalue):
        outln ="<"+tagname+">"+str(tagvalue)+"</"+tagname+">"
        print self.indentstr+outln
        
    def writeBooleanTag(self,tagname,tagvalue):
        if( tagvalue ) :
            outln ="<"+tagname+">true</"+tagname+">"
            print self.indentstr+outln
        else:
            outln ="<"+tagname+">false</"+tagname+">"
            print self.indentstr+outln

    def writeListTag(self,tagname,tagvalue):
        xstr=[]
        for x in tagvalue:
            xstr.append(str(x))
            xstr.append(" ")
        outln = "<"+tagname+">"+(''.join(xstr)).strip()+"</"+tagname+">"
        print self.indentstr+outln
        
    def openTag(self,tagname):
        outln = "<"+tagname+">"
        print self.indentstr+outln
        self.indents.append("  ")
        self.tagnames.append(tagname)
        self.indentstr=''.join(self.indents)
        
    def closeTag(self):
        outln = "</"+self.tagnames.pop()+">"
        self.indents.pop()
        self.indentstr=''.join(self.indents)
        print self.indentstr+outln
