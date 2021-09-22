    std::cout << "sizeof(arr) = " << sizeof(arr) << " sizeof(*arr) = " << sizeof(*arr) << " sizeof(int) = " << sizeof(int) << " sizeof(int*) = " << sizeof(int*) << std::endl; 
    std::cout << sizeof(arr) / sizeof(*arr) << std::endl;
    std::cout << sizeof(arr) / sizeof(int*) << std::endl;
    
    std::cout << std::endl;