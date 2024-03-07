#include <QApplication>
#include <QLabel>
#include <QWindow>
#include <QWidget>

/* Luna Steed S24
**** TODO:
*		Store a renderer object
*       Store an unordered map of QWidget Pointers
*		Method to display a menu that takes in a key from QT Slot and displays a menu. -- Interact with MenuMaster
*		Store MenuMaster object
*/

class GameWindow : public QWindow {

	public:
		Renderer renderer = new Renderer()
};