#ifndef NODESVIEW_H
#define NODESVIEW_H

#include <QGraphicsView>

class AbstractNode;
class LinkItem;
class PackageView;

namespace nodes {
class Node;
} // namespace nodes

class NodesView : public QGraphicsView {
  Q_OBJECT

 public:
  NodesView(QGraphicsScene *const a_scene, PackageView *a_parent);
  ~NodesView() override;

  void dragEnterEvent(QDragEnterEvent *a_event) override;
  void dragLeaveEvent(QDragLeaveEvent *a_event) override;
  void dragMoveEvent(QDragMoveEvent *a_event) override;
  void dropEvent(QDropEvent *a_event) override;
  void keyReleaseEvent(QKeyEvent *a_event) override;
  void wheelEvent(QWheelEvent *a_event) override;

  void mouseMoveEvent(QMouseEvent *a_event) override;

  LinkItem *dragLink() const { return m_dragLink; }
  void setDragLink(LinkItem *a_link);
  void acceptDragLink();
  void cancelDragLink();

  PackageView *packageView() const { return m_packageView; }

 private:
  PackageView *m_packageView{};
  nodes::Node *m_dragNode{};
  LinkItem *m_dragLink{};
  int32_t m_scheduledScalings{};
};

#endif // NODESVIEW_H