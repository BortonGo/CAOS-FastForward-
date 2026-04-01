#!/usr/bin/env python3
import random
import struct
import sys
from dataclasses import dataclass
from typing import Optional


@dataclass
class Node:
    value: int
    left: Optional['Node'] = None
    right: Optional['Node'] = None
    index: Optional[int] = None

    def left_index(self):
        return self.left.index if self.left else 0

    def right_index(self):
        return self.right.index if self.right else 0

    def to_binary(self):
        return struct.pack("<iii", self.value, self.left_index(), self.right_index())


n, min_, max_, seed = map(int, sys.argv[1:])

random.seed(seed)
values = sorted(random.choices(range(min_, max_ + 1), k=n))
nodes = []

def make_tree(arr):
    global nodes
    if not arr:
        return None
    mid = len(arr) // 2
    root = Node(
        arr[mid],
        make_tree(arr[:mid]),  # extra copies (n log n memory and time, should be ok)
        make_tree(arr[mid + 1:])
    )
    nodes.append(root)
    return root

make_tree(values)

*nodes, root = nodes
random.shuffle(nodes)
nodes = [root] + nodes
for i, node in enumerate(nodes):
    node.index = i

for node in nodes:
    sys.stdout.buffer.write(node.to_binary())
