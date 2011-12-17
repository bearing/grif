#include <vector>
// GCG param list
private: int shapingtime;

// GCG param getters
template <class T> T getParam(QString name) {
  if (name == "shapingtime") { return this->shapingtime; }
}

// GCG param getters (QVector)
template <class T> QVector<T> getQVecParam(QString name) {
}

// GCG param getters (vector)
template <class T> std::vector<T> getStdVecParam(QString name) {
}

// GCG param setters
template <class T> void setParam(QString name, T value) {
  if (name == "shapingtime") { this->shapingtime = value; }
}

// GCG param setters (QVector)
template <class T> void setQVecParam(QString name, QVector<T> value) {
}

// GCG param setters (vector)
template <class T> void setStdVecParam(QString name, std::vector<T> value) {
}

// GCG default value sets
void setInitialGCGValues() {
  shapingtime = 800;
}

// GCG action list
// PLEASE IMPLEMENT IN A SOURCE FILE. If code gen runs again, the implementation may be overwritten!
void Reconfigure();

// GCG action chocie
void runAction(QString name) {
  if (false) { /*dummy holder for code generation*/ }
  else if (name == "Reconfigure") { Reconfigure(); }
  else { std::cout << "could not parse action"; }
}

//GCG print actions (for GRICLI)
void printActions() {
  std::cout << "Reconfigure()" << endl;
}
