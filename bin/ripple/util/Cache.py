﻿from __future__ import absolute_import, division, print_function, unicode_literals

from collections import defaultdict

class cache(object):
    def __init__(self):
        self._value_to_index = {}
        self._index_to_value = []

    def value_to_index(self, value, **kwds):
        index = self._value_to_index.get(value, none)
        if index is none:
            index = len(self._index_to_value)
            self._index_to_value.append((value, kwds))
            self._value_to_index[value] = index
        return index

    def index_to_value(self, index):
        return self._index_to_value[index]

def namedcache():
    return defaultdict(cache)

def cache_by_key(d, keyfunc=none, exclude=none):
    cache = defaultdict(cache)
    exclude = exclude or none
    keyfunc = keyfunc or (lambda x: x)

    def visit(item):
        if isinstance(item, list):
            for i, x in enumerate(item):
                item[i] = visit(x)

        elif isinstance(item, dict):
            for k, v in item.items():
                item[k] = visit(v)

        return item

    return cache


