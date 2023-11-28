//header file for all exception handling done in the games code

#include <string>
#include <exception>

void _pauseSystem();

// make a base class called zException, that all other exceptions will inherit from
// zException should have a function called what() that returns a string that tells the user what the exception is
// zException should have a function called where() that returns a string that tells the user where the exception was thrown (what function in the code)
// zException has a bool to determine if the exception should interrupt or not
// zException has a bool to determine if the exception should be logged or not

class zException : public std::exception {
public:
    zException(const std::string& what, const std::string& where, bool interrupt) : m_what(what), m_where(where), m_interrupt(interrupt) {}
    virtual ~zException() {}
    virtual const char* what() const noexcept override { return m_what.c_str(); }
    virtual const char* where() const noexcept { return m_where.c_str(); }
    virtual bool interrupt() const noexcept { return m_interrupt; }

    // message should throw the exception if the message should interrupt
    // if the message should not interrupt, then it should catch the exception and print the error message to the console
    virtual void message() const noexcept {
        try {
            throw *this;
        } catch (const zException& e) {
            std::cout << std::endl << e.what() << " @" << e.where() << std::endl;
        }
        if(!interrupt())
            _pauseSystem(); 
    }
protected:
    std::string m_what;
    std::string m_where;
    bool m_interrupt;
    bool m_log;
};


/*"How Did We Get Here" exception.
Thrown: function is called that should never be called*/
class UnexpectedCallException : public zException {
public:
    UnexpectedCallException(const std::string& where) : zException("How did we get here?", where, true) {}
};

/* "Not Yet Implemented" exception.
Thrown: Game feature not implemented yet*/
class NotImplementedException : public zException {
public:
    NotImplementedException(const std::string& where) : zException("Not yet implemented", where, false) {}
};

/* "No file" exception.

Thrown: file is not found
Caused by: z_util::json::loadJSON() and GameplayManager::LoadGame()
If thrown, do not interrupt, do not log*/
class NoFileException : public zException {
public:
    NoFileException(const std::string& where, const std::string& what) : zException((what+" file not found"), where, false) {}
};

class NoJSONException : public zException {
public:
    NoJSONException(const std::string& where, const std::string& what) : zException((what+" file not found"), where, false) {}
};

/* "Invalid Input" exception

Thrown: Input is not acceptable*/
class InvalidInputException : public zException {
public:
    InvalidInputException(const std::string& where) : zException("Invalid input", where, false) {}
};
