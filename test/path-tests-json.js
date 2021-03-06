﻿module.exports = {
  "path tests #1 (xrp -> xrp) and #2 (xrp -> iou)": {

    "ledger": {"accounts": {"a1": {"balance": ["100000.0",
                                               "3500/xyz/g1",
                                               "1200/abc/g3"],
                                   "trusts": ["5000/xyz/g1",
                                              "5000/abc/g3"]},
                            "a2": {"balance": ["10000.0"],
                                   "trusts": ["5000/xyz/g2",
                                              "5000/abc/g3"]},
                            "a3": {"balance": ["1000.0"],
                                   "trusts": ["1000/abc/a2"]},
                            "g1": {"balance": ["1000.0"]},
                            "g2": {"balance": ["1000.0"]},
                            "g3": {"balance": ["1000.0"]},
                            "m1": {"balance": ["1000.0",
                                               "25000/xyz/g2",
                                               "25000/abc/g3"],
                                   "offers": [["1000/xyz/g1",
                                               "1000/xyz/g2"],
                                              ["10000.0",
                                               "1000/abc/g3"]],
                                   "trusts": ["100000/xyz/g1",
                                              "100000/abc/g3",
                                              "100000/xyz/g2"]}}},

    "paths_expected": {"t1": {"a1": {"n_alternatives": 0,
                                     "src": "a1",
                                     "send": "10.0",
                                     "dst": "a2",
                                     "via": "xrp"},
                              "a2": {"comment": "send to non existing account",
                                     "src": "a1",
                                     "send_comment": "malformed error not great for 10.0 amount",
                                     "send": "200.0",
                                     "dst": "rbmhuvavi372aerwzwergjhljqkmmawxx",
                                     "via": "xrp",
                                     "n_alternatives": 0}},
                       "t2": {"a": {"alternatives": [{"amount": "100.0",
                                                           "paths": [
                                                            ["abc/g3|$"]
                                                           ]}],
                                    "src": "a2",
                                    "send": "10/abc/g3",
                                    "dst": "g3",
                                    "via": "xrp",
                                    "debug": 0,
                                    "n_alternatives": 1},
                              "b": {"alternatives": [{"amount": "10.0",
                                                      "paths": [["abc/g3|$",
                                                                 "abc/g3|g3"]]}],
                                    "src": "a1",
                                    "send": "1/abc/a2",
                                    "dst": "a2",
                                    "via": "xrp",
                                    "n_alternatives": 1},
                              "c": {"alternatives": [{"amount": "10.0",
                                                      "paths": [["abc/g3|$",
                                                                 "abc/g3|g3",
                                                                 "abc/a2|a2"]]}],
                                    "src": "a1",
                                    "send": "1/abc/a3",
                                    "dst": "a3",
                                    "via": "xrp",
                                    "n_alternatives": 1}}}},
 "path tests #3 (non-xrp to xrp)": {

    "ledger": {"accounts": {"a1": {"balance": ["1000.0",
                                               "1000/abc/g3"]},
                            "a2": {"balance": ["1000.0",
                                               "1000/abc/g3"]},
                            "g3": {"balance": ["1000.0"]},
                            "m1": {"balance": ["11000.0",
                                               "1200/abc/g3"],
                                   "offers": [["1000/abc/g3",
                                               "10000.0"]],
                                   "trusts": ["100000/abc/g3"]}}},

    "paths_expected": {"t3": {"a": {"alternatives": [{"amount": "1/abc/a1",
                                                      "paths": [["abc/g3|g3",
                                                                 "xrp|$"]]}],
                                    "src": "a1",
                                    "dst": "a2",
                                    "debug":false,
                                    "send": "10.0",
                                    "via": "abc"}}}}
}


