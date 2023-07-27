# Document Autorisé C++ 2A

## Table of Contents
- [Introduction](#introduction)
- [Classes](#classes)
  - [Devoir Example](#devoir)
    - [Copy Constructor](#copy-constructor)
    - [Destructor](#destructor)
    - [Operator= Constructor](#operator=)
  - [Developpeur Example](#class-2)
    - [Search](#search)
    - [Add](#add)
    - [Remove](#remove)
- [Conversions](#conversions)
- [Useful Typeid](#useful-typeid)
- [Example Classes](#example-classes)
  - [Base Class Example](#base-class-example)
    - [Header](#header)
    - [Implementation](#implementation)
  - [Derived Class Example](#derived-class-example)
    - [Header](#header-1)
    - [Implementation](#implementation-1)
- [Useful Methods](#useful-methods)
  - [Operator= Overloading](#operator-overloading)
  - [Operator+= Overloading](#operator-overloading-1)
  - [Operator+ Overloading (Class + Class)](#operator-overloading-2)
  - [Operator+ Overloading (Class + Int)](#operator-overloading-3)
  - [Operator+ Overloading (Int + Class)](#operator-overloading-4)
  - [Operator<< Overloading (Friend)](#operator-overloading-5)
  - [Operator>> Overloading (Friend)](#operator-overloading-6)
  - [Operator>= Overloading](#operator-overloading-7)
  - [Save to File](#save-to-file)
  - [Read from File (Line by Line)](#read-from-file-line-by-line)
  - [Read from File (Char by Char)](#read-from-file-char-by-char)
  - [Read from File (Word by Word)](#read-from-file-word-by-word)
  - [Try Throw Catch Example (int)](#try-throw-catch-example-int)
  - [Try Throw Catch Example (string)](#try-throw-catch-example-string)

## Introduction
This is a "Document Autorisé" for my C++ exam. Made by Jev1337 / AbdelMalek Amir Hassen.
Just replace the names of your class with some of the examples and you are good to go.

## Classes

### Devoirs

#### Copy-Constructor
```cpp
Application::Application(const Application& a)
{
	l = a.l;
    /*
        Don't forget other attributes such as:

        nom = a.nom;
        someList = a.someList;
    */
	list<Epreuve*>::const_iterator i;
	for (i = a.l.begin(); i != a.l.end(); ++i)
	{
		if (typeid(**i) == typeid(EpreuvePratique)) {
			l.push_back(new EpreuvePratique(static_cast<EpreuvePratique&>(**i)));
		}
		else
			l.push_back(new Quiz(static_cast<Quiz&>(**i)));
	}
}
```

#### Destructor
```cpp
Application::~Application(){
	list<Epreuve*>::iterator i;
	for (i = l.begin(); i != l.end(); ++i)
		delete *i;
}
```

#### Operator=
```cpp
Application& Application::operator=(const Application& a)
{
	if (this != &a)
	{
		l = a.l;
        /*
            Don't forget other attributes such as:
        
            nom = a.nom;
            someList = a.someList;
        */
		list<Epreuve*>::iterator j;
		for (j = l.begin(); j != l.end(); ++j)
			delete *j;

		list<Epreuve*>::const_iterator i;
		for (i = a.l.begin(); i != a.l.end(); ++i)
		{
			if (typeid(**i) == typeid(EpreuvePratique))
				l.push_back(new EpreuvePratique(static_cast<EpreuvePratique&>(**i))); 
			else
				l.push_back(new Quiz(static_cast<Quiz&>(**i)));
		}
	}
	return *this;
}
```

### Developpeur

#### Search
```cpp
list<Developpeur*>::iterator Application::chercherDev(int CIN){
	list<Developpeur*>::iterator it;
	for (it = devs.begin(); it != devs.end() && (*it)->getCIN() != CIN; ++it);
	return it;
}
```
#### Add
```cpp
void Application::AjouterDev(const Developpeur& dev){

	if (chercherDev(dev.getCIN()) == devs.end()) {
		devs.push_back(new Developpeur(dev));
	}
	else
		cout << "Erreur: Ce compte Developpeur existe deja avec le meme email / CIN" << endl;
}
```
#### Remove
```cpp
void Application::supprimerQuiz(string theme)
{
	list<Epreuve*>::iterator i;
	for (i = l.begin(); i != l.end(); ++i)
	{
		if (typeid(**i) == typeid(Quiz))
		{
			Quiz* q = static_cast<Quiz*>(*i);
			if (q->getTheme() == theme)
			{
				i = l.erase(i);
				i--;
			}
		}
	}
}
```
## Conversions
```cpp
EpreuvePratique* e = static_cast<EpreuvePratique*>(*i); // Converts *i to EpreuvePratique*
Quiz* q = static_cast<Quiz*>(*i); // Converts *i to Quiz*
Quiz* q = (Quiz*)(*i); // Converts *i to Quiz* (Old C-style cast)
```

## Useful Typeid
```cpp
if (typeid(**i) == typeid(EpreuvePratique)) // Checks if *i is an EpreuvePratique
if (typeid(**i) == typeid(Quiz)) // Checks if *i is a Quiz
```

## Example Classes

### Base Class Example

#### Header
```cpp
class Developpeur
{
public:
	Developpeur();
	Developpeur(int, string, string, string, double);
	virtual ~Developpeur();
    //...
    virtual void afficher();
    //...
private:
    //...
};
```

#### Implementation
```cpp
Developpeur::Developpeur(){

}
Developpeur::Developpeur(int cin, string nom, string prenom, string email , double salaire) {
	this->CIN = cin;
	this->nom = nom;
	this->prenom = prenom;
	this->email = email;
	this->salaire = salaire;
}
Developpeur::~Developpeur(){

}

void Developpeur::afficher(){
    //... cout here
}

```

### Derived Class Example

#### Header
```cpp
class SousTraitant : public Developpeur
{
public:
	SousTraitant();
	SousTraitant(int, string, string, string, double, string, int, double);
	~SousTraitant();
    //...
    void afficher();
    //...
private:
    //...
};
```

#### Implementation
```cpp
SousTraitant::SousTraitant(){

}

SousTraitant::SousTraitant(int cin, string nom, string prenom, string email, double salaire, string nomSoc, int nbHeure, double prixHeure) : Developpeur(cin, nom, prenom, email, salaire) {
    this->nomSoc = nomSoc;
    this->nbHeure = nbHeure;
    this->prixHeure = prixHeure;
}

SousTraitant::~SousTraitant(){

}

void SousTraitant::afficher(){
    Developpeur::Afficher();
    //cout here...
}
```


## Useful Methods

### Operator= Overloading
```cpp
aRandomClass aRandomClass::operator=(int item)
{
    this->item = item;
    return *this;
}
```

### Operator+= Overloading
```cpp
aRandomClass aRandomClass::operator+=(const aRandomClass& a)
{
    this->item += a.item;
    return *this;
}

```


### Operator+ Overloading (Class + Class)
```cpp
aRandomClass aRandomClass::operator+(const aRandomClass& a)
{
    aRandomClass temp(this->item + a.item);
    return temp;
}
```

### Operator+ Overloading (Int + Class)
```cpp
aRandomClass operator+(int item)
{
    aRandomClass temp(item + this->item);
    return temp;
}
```

### Operator+ Overloading (Class + Int)
```cpp
//Header.h
friend aRandomClass operator+(int item, const aRandomClass& a);

//Source.cpp
aRandomClass operator+(int item, const aRandomClass& a)
{
    aRandomClass temp(item + a.item);
    return temp;
}
```


### Operator<< Overloading (Friend)
```cpp
//Header.h
friend ostream& operator<<(ostream& out, const aRandomClass& a);

//Source.cpp
ostream& operator<<(ostream& out, const aRandomClass& a)
{
    out << a.item;
    return out;
}
```


### Operator>> Overloading (Friend)
```cpp
//Header.h
friend istream& operator>>(istream& in, aRandomClass& a);

//Source.cpp
istream& operator>>(istream& in, aRandomClass& a)
{
    in >> a.item;
    return in;
}
```

### Operator>= Overloading
```cpp
bool aRandomClass::operator>=(const int& item) {
	return this->item >= item;
}
```


### Save to File
```cpp
void saveToFile(string fileName, string text)
{
    ofstream file(fileName);
    file << text << endl;
    file.close();
}
```


### Read from File (Line by Line)
```cpp
void readFromFile(string fileName)
{
    ifstream file(fileName);
    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    file.close();
}
```


### Read from File (Char by Char)
```cpp
void readFromFile(string fileName)
{
    ifstream file(fileName);
    char c;
    while (file.get(c))
    {
        cout << c;
    }
    file.close();
}

```


### Read from File (Word by Word)
```cpp
void readFromFile(string fileName)
{
    ifstream file(fileName);
    string word;
    while (file >> word)
    {
        cout << word << endl;
    }
    file.close();
}
```


### Try Throw Catch Example (int)
```cpp
void tryThrowCatchExample()
{
    try
    {
        throw 20;
    }
    catch (int e)
    {
        cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
}
```


### Try Throw Catch Example (string)
```cpp
void tryThrowCatchExample()
{
    try
    {
        throw string("Exception");
    }
    catch (string e)
    {
        cerr << "An exception occurred. Exception Nr. " << e << '\n';
    }
}
```


## Conclusion
I hope this comes to be useful! It has taken me some time to make this look like this!

