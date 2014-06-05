#ifndef CACTUSCORE_SWATCH_BOARD_BOARDFACTORY_H
#define CACTUSCORE_SWATCH_BOARD_BOARDFACTORY_H


// SWATCH HEADERS


// OTHER HEADERS


// C++ HEADERS
#include <map>
#include <vector>
#include <string>


// FORWARD DECLARATIONS
namespace swatch
{

namespace board
{
class Board;



// BOARDFACTORY CLASS

/**
 * A class to construct a Board object based on the type of uTCA IP end point found in a permanent storage media
 * The class implements a Factory pattern. Since the BoardFactory object is created using a singleton idiom, the class should
 * be mutex protected if it is going to be used in multithreaded environments
 */
class BoardFactory
{
public:

	/**
	 * static method to get the single instance of this class. It creates the instance if it doesn't exist before and also
	 * the different Creator objects for every type of board
	 * @return single instance of this class
	 */
	static BoardFactory& getInstance();

	/**
	 *	Adds a creator object to a board type, replacing it if it existed before. The Creator will build objects of class "T"
	 *	@param boardType String containing the type of board associated with a particular creator
	 */
	template <class T> void add(const std::string& boardType)
	{
		std::map<std::string, CreatorInterface*>::iterator productIt = mapBoardTypeToBoardCreator_.find(boardType);
		if (productIt != mapBoardTypeToBoardCreator_.end())
		{
			delete productIt->second;
		}

		mapBoardTypeToBoardCreator_[boardType] = new Creator<T>();
	}

	/**
	 * Creates a board object based on the board type
	 * @param boardType The board type that has to be created
	 * @return pointer to the board object
	 */
	Board* getBoard(const std::string& boardType);

	/**
	 * Informs about the different types of boards that have been already registered in this class
	 * @return vector of strings that contains the different types of boards registered in this class
	 */
	std::vector<std::string> getRegisteredBoardTypes();


protected:
private:


	//! Abstract class that defines the interface for the Creator objects
	class CreatorInterface
	{
	public:

		/**
		 * Constructor
		 */
		CreatorInterface()
		{
		}

		/**
		 * Destructor
		 */
		virtual ~CreatorInterface()
		{
		}

		/**
		 * Abstract method to build Creator objects based on the type of board to be created
		 * @param boardType The type of board to be created
		 * @return A pointer to an specific Board object
		 */
		virtual Board* create(const std::string& boardType) = 0;
	};


	// ! Template class that implements the CreatorInterface
	template <class T>
	class Creator : public CreatorInterface
	{
	public:

		/**
		 * Constructor
		 */
		Creator()
		{
		}

		/**
		 * Destructor
		 */
		virtual ~Creator()
		{
		}

		/**
		 * Implementation of the CreatorInterface method. Creates a specific Board object based on the board type
		 * @param boardType Type of board to be created
		 * @return Pointer to the specific board's base class (Board)
		 */
		Board* create(const std::string& boardType)
		{
			Board* b = new T(boardType);
			return b;
		}
	};



	/**
	 * @param
	 * @return
	 */
	std::string getBoardTypeFromName(const std::string& boardName);


	/**
	 * Constructor Made private to prevent BoardFactory objects from being created
	 */
	BoardFactory();

	/**
	 * Copy constructor Made private to prevent BoardFactory objects from being created
	 */
	BoardFactory(const BoardFactory& bf);

	/**
	 * Destructor
	 */
	virtual ~BoardFactory();

	/**
	 * Assignement operator Made private to prevent BoardFactory objects from being created
	 */
	BoardFactory& operator=(const BoardFactory& bf);

	//! The pointer to the BoardFactory object, made static to have just one instance of the class
	static BoardFactory* bf_;

	//! map associating a board type with the board's particular creator
	static std::map<std::string, CreatorInterface*> mapBoardTypeToBoardCreator_;





};


} // end ns board
} // end ns swatch
#endif
