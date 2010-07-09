class console
{
    public:
        console() {};
        virtual ~console() {};
        bool parseCommandLine();
        void print(const std::string & strText);
    public:
        std::string	m_commandLine;
    };




bool console::parseCommandLine()
    {
        std::string::size_type index = 0;
        print(m_commandLine);
        return true;
    };


class myConsole: public console
{
    public:
    myConsole() ;
    ~myConsole() ;
};

