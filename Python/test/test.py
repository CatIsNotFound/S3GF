import sys
import os

def init():
    module_path = r'd:\Coding\MyEngine\cmake-build-debug-visual-studio\Python'
    if module_path not in sys.path:
        sys.path.insert(0, module_path)


if __name__ == '__main__':
    init()
    import PyEngine as E

    help(E)





