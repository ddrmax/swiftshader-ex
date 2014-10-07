//===- subzero/src/IceTimerTree.h - Pass timer defs -------------*- C++ -*-===//
//
//                        The Subzero Code Generator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TimerTree class, which allows flat and
// cumulative execution time collection of call chains.
//
//===----------------------------------------------------------------------===//

#ifndef SUBZERO_SRC_ICETIMERTREE_H
#define SUBZERO_SRC_ICETIMERTREE_H

#include "IceTimerTree.def"

namespace Ice {

class TimerTreeNode;

// Timer tree index type
typedef std::vector<TimerTreeNode>::size_type TTindex;

// TimerTreeNode represents an interior or leaf node in the call tree.
// It contains a list of children, a pointer to its parent, and the
// timer ID for the node.  It also holds the cumulative time spent at
// this node and below.  The children are always at a higher index in
// the TimerTreeNode::Nodes array, and the parent is always at a lower
// index.
class TimerTreeNode {
public:
  TimerTreeNode() : Parent(0), Interior(0), Time(0) {}
  std::vector<TTindex> Children; // indexed by TimerIdT
  TTindex Parent;
  TimerIdT Interior;
  double Time;
};

class TimerStack {
  // TimerStack(const TimerStack &) = delete;
  TimerStack &operator=(const TimerStack &) = delete;

public:
  enum TimerTag {
#define X(tag) TT_##tag,
    TIMERTREE_TABLE
#undef X
        TT__num
  };
  TimerStack(const IceString &Name);
  TimerIdT getTimerID(const IceString &Name);
  void push(TimerIdT ID);
  void pop(TimerIdT ID);
  void dump(Ostream &Str, bool DumpCumulative);

private:
  void update();
  static double timestamp();
  const IceString Name;
  const double FirstTimestamp;
  double LastTimestamp;
  uint64_t StateChangeCount;
  // IDsIndex maps a symbolic timer name to its integer ID.
  std::map<IceString, TimerIdT> IDsIndex;
  std::vector<IceString> IDs;        // indexed by TimerIdT
  std::vector<TimerTreeNode> Nodes;  // indexed by TTindex
  std::vector<double> LeafTimes;     // indexed by TimerIdT
  TTindex StackTop;
};

} // end of namespace Ice

#endif // SUBZERO_SRC_ICETIMERTREE_H
