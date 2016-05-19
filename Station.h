/*-------------------------------------------------------------------------*
 *---									---*
 *---		Station.h						---*
 *---									---*
 *---	    This file declares a class that implements a Station	---*
 *---	object at which Train objects arrive, pause, and then leave.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		Joseph Phillips		2016 May 5	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class   Station
{
    //  I.  Member vars:
    //  PURPOSE:  To hold the name of '*this' Station.
    std::string		name_;
    
    //  PURPOSE:  To point to the Train object currently at '*this' Station
    //	(if there is one).
    Train*		trainPtr_;
    pthread_mutex_t safe;
    pthread_cond_t  canArrive;
    public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To initialize '*this' Station to have name 'newName' and
    //	and no Train.  No return value.
    Station		(const std::string&	newName
                 )
    {
        pthread_mutex_init(&safe,NULL);
        pthread_cond_init(&canArrive,NULL);
        name_	= newName;
        trainPtr_	= NULL;
    }
    
    //  PURPOSE:  To release resources.  No parameters.  No return value.
    ~Station		()
    {
        pthread_mutex_destroy(&safe);
        pthread_cond_init(&canArrive,NULL);
    }
    
    //  PURPOSE:  To return the name of '*this' Station.  No parameters.
    const std::string&
    getName	()
    const
    {
        return(name_);
    }
    
    //  PURPOSE:  To return a pointer to the Train object currently at '*this'
    //	Station (if there is one).
    Train*	getTrainPtr
    ()
    const
    {
        return(trainPtr_);
    }
    
    //  PURPOSE:  To let '*newTrainPtr' arrive at '*this' Station.  No return
    //	value.
    void		arrive	(Train*	newTrainPtr
                         )
    {
        pthread_mutex_lock(&safe);
        while  (getTrainPtr() != NULL)
        {
            std::cout << getTrainPtr()->getName()
            << " is at " << getName()
            << ", "	     << newTrainPtr->getName()
            << " must wait.\n";
            usleep(10) + rand() % 10;
            pthread_cond_wait(&canArrive,&safe);
        }
        
        std::cout << newTrainPtr->getName() << " arriving at " << getName() << "\n";
        
        if  (getTrainPtr() != NULL)
            std::cout << newTrainPtr->getName() << " and "
            << getTrainPtr()->getName() << " CRASHED at "
            << getName() << std::endl;
        
        trainPtr_	= newTrainPtr;
        
        usleep(10 + rand() % 10);
        pthread_mutex_unlock(&safe);
    }
    
    //  PURPOSE:  To let the Train object currently at '*this' Station leave.
    //	Returns pointer to departing Train.
    Train*	leave	()
    {
        pthread_mutex_lock(&safe);
        while  (getTrainPtr() == NULL)
        {
            std::cout << "No train at " << getName() << "!\n";
            usleep(10 + rand() % 10);
        }
        
        Train*	toReturn	= getTrainPtr();
        
        std::cout << toReturn->getName() << " leaving " << getName() << "\n";
        
        usleep(10 + rand() % 10);
        trainPtr_ = NULL;
        
        pthread_cond_signal(&canArrive);
        pthread_mutex_unlock(&safe);
        return(toReturn);
    }
    
};