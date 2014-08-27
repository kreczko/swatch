#!/usr/bin/env python

import threading
import subprocess
import time
import uhal

uhal.setLogLevelTo(uhal.LogLevel.ERROR)

class DummyHWThread( threading.Thread ):

    def __init__( self, name, port, addr, values={} ):
        threading.Thread.__init__(self)
        self.name = name
        self.port = port
        self.addr = addr
        self._values = values
        self._terminate = False
        self._kill = False
        self._code = None
        self._subp = None
        self.hw = None

    def __del__(self):
        # print self._subp.poll();
        if self._subp and self._subp.poll() == None:
            self._subp.kill()


    def run(self):

        cmd = ['/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe','-p'+str(self.port), '-v2','-V']
        self._subp = subprocess.Popen(cmd, cwd='/tmp')
        dummyhw = self._subp

        # print "Let it run for 1 sec"
        time.sleep(1.)

#        if self._values:
        hw = uhal.getDevice('dummy_'+self.name,'ipbusudp-2.0://localhost:'+str(self.port), self.addr)

        for k,v in self._values.iteritems():
            hw.getNode(k).write(v)

        hw.dispatch()
        
        self.hw = hw

        retcode = None
        while( not retcode ):
            retcode = dummyhw.poll()

            if retcode is not None:
                self.code = retcode
                break
            
            if self._kill:
                try:
                    print self._name,'Game over!'
                    dummyhw.kill()
                except OSError:
                    pass
                break
            else:
                time.sleep(0.1)

    def kill(self):
        self._kill = True

    def poll(self):
        if not self._subp:
            return None
        return self._subp.poll()


class PuppetMaster:
    def __init__(self):
        self._dummies = {}

    def add(self,d):
        if d.name in dummies:
            raise RuntimeError(d.name+" exists")
          
        self._dummies[d.name] = d
        
    def get(self, name):
#        if name not in self._dummies:
#            raise KeyError("dummy "+name+" not found")
        return self._dummy[name]
            

    def start(self):
        for d in self._dummies.itervalues():
            print 'Starting',d.name
            d.start()
            print d.name,'started'

    def kill(self):
        for d in self._dummies.itervalues():
            print 'Killing',d.name
            d.kill()
        print 'Wait for '
        while threading.activeCount() > 1:
            time.sleep(.1)

    def monitor(self):

        while threading.activeCount() > 1:

            for d in self._dummies.itervalues():
                r = d.poll()
                if r is not None:
                    print d.name,"terminated with code",r

            time.sleep(1)

if __name__ == "__main__":

    addr="file:///opt/cactus/etc/uhal/tests/dummy.xml"

    pm = PuppetMaster()

    pm.add( DummyHWThread( 'dummy1', 50010, addr, {'ctrl.id.magic':0xc0de1234} ) )
    pm.add( DummyHWThread( 'dummy2', 50011, addr, {'ctrl.id.magic':0xc0de4567} ) )

    try:
        pm.start()
        pm.monitor()
    except KeyboardInterrupt:
        pm.kill()
