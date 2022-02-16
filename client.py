import sys
import socket
import json
import os
import threading
import random
from stomp import *

# class receiver(object):
#     def __init__(self, ip, port):
#         try:
#             socket.inet_aton(ip)
#             if 0 < int(port) < 65535:
#                 self.ip = ip
#                 self.port = int(port)
#             else:
#                 raise Exception('Port value should between 1~65535')
#             self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#         except Exception as e:
#             print(e, file=sys.stderr)
#             sys.exit(1)

#     def run(self):
#         self.sock.bind((self.ip, self.port))
#         self.sock.listen(100)
#         #socket.setdefaulttimeout(0.1)
#         while True:
#             try:
#                 conn, addr = self.sock.accept()
#                 with conn:
#                     cmd = conn.recv(4096).decode()
#                     print(cmd)
#                     resp = self._check_end(cmd)
#                     conn.send(cmd)
#             except Exception as e:
#                 print(e, file=sys.stderr)
#                 sys.exit(1)

#     def _check_end(self, cmd):
#         command = cmd.split()
#         if command[0] == 'exit':
#             sys.exit(1)
#         else:
#             return

# def receiving(ip, port):
#     c = receiver(ip, str(port))
#     c.run()

class MyListener(ConnectionListener):
    def on_message(self, headers, body):
        print(body)
        pass

class Client(object):
    def __init__(self, ip, port):
        try:
            socket.inet_aton(ip)
            if 0 < int(port) < 65535:
                self.ip = ip
                self.port = int(port)
            else:
                raise Exception('Port value should between 1~65535')
            self.cookie = {}
            self.cracker = {}
        except Exception as e:
            print(e, file=sys.stderr)
            sys.exit(1)

    def run(self, ip, port, ck):
        while True:
            cmd = sys.stdin.readline()
            if cmd == 'exit\n':
                return
            if cmd != os.linesep:
                try:
                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                        s.connect((self.ip, self.port))
                        cmd = self.__attach_token(cmd)
                        print('-'+cmd+'-')
                        s.send(cmd.encode())
                        resp = s.recv(4096).decode()
                        print(resp)
                        self.__show_result(json.loads(resp), cmd, ck)
                except Exception as e:
                    print(e, file=sys.stderr)

    def __show_result(self, resp, cmd=None, ck=None):
        if 'message' in resp:
            print(resp['message'])

        if 'invite' in resp:
            if len(resp['invite']) > 0:
                for l in resp['invite']:
                    print(l)
            else:
                print('No invitations')

        if 'friend' in resp:
            if len(resp['friend']) > 0:
                for l in resp['friend']:
                    print(l)
            else:
                print('No friends')

        if 'post' in resp:
            if len(resp['post']) > 0:
                for p in resp['post']:
                    print('{}: {}'.format(p['id'], p['message']))
            else:
                print('No posts')
        
        if 'group' in resp:
            if len(resp['group']) > 0:
                for l in resp['group']:
                    print(l)
            else:
                print('No groups')
        
        if 'joined' in resp:
            if len(resp['joined']) > 0:
                for l in resp['joined']:
                    print(l)
            else:
                print('No groups')

        if cmd:
            command = cmd.split()
            if resp['status'] == 0 and command[0] == 'login':
                self.cookie[command[1]] = resp['token']
                self.cracker[resp['token']] = command[1]
            if 'subs' in resp:
                if len(resp['subs']) > 0:
                    for p in resp['subs']:
                        ck.subscribe(p, command[1])
            if resp['status'] == 0 and command[0] == 'logout':
                ck.unsubscribe(self.cracker[command[1]])

    def __attach_token(self, cmd=None):
        if cmd:
            command = cmd.split()
            if len(command) > 1:
                if command[0] != 'register' and command[0] != 'login':
                    if command[1] in self.cookie:
                        command[1] = self.cookie[command[1]]
                    else:
                        command.pop(1)
            return ' '.join(command)
        else:
            return cmd

def launch_client(ip, port):
    c = Client(ip, port)
    ck = Connection([(ip, 61613)])
    ck.set_listener('', MyListener())
    # ck.set_listener('', PrintingListener())
    ck.start()
    ck.connect('admin', 'password', wait=True)
    c.run(ip, port, ck)

if __name__ == '__main__':
    if len(sys.argv) == 3:
        launch_client(sys.argv[1], sys.argv[2])
    else:
        print('Usage: python3 {} IP PORT'.format(sys.argv[0]))
