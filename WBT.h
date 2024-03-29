//
//  WBT.h
//
//  Jean Goulet 2014-11-05.
//  Copyleft (c) 2014 UdeS
//
//  Modification par:
//      Ian Oscar Vasquez Gutierrez     12 202 430
//      Alexandre Blais                 matricule
//

#ifndef WBT_WBT_h
#define WBT_WBT_h
#include <vector>
#include <string>
#include <cassert>

template <typename TYPE>
class WBT{
public:
    class iterator;
    friend class iterator;
private:
    struct noeud{
        TYPE CONTENU;
        noeud *PARENT,*GAUCHE,*DROITE;
        size_t POIDS;
        noeud(const TYPE& c,noeud* PAR,noeud*GAU=nullptr,noeud*DRO=nullptr):CONTENU(c),PARENT(PAR),GAUCHE(GAU),DROITE(DRO),POIDS(1){}
        noeud():PARENT(nullptr),GAUCHE(nullptr),DROITE(nullptr),POIDS(0){}
    };
    noeud RACINE;
    noeud* FIN;
    //fonctions privees
    void vider(noeud*&);
    void copier(noeud*,noeud*&,noeud*);
    bool insert(const TYPE&,noeud*&,iterator&);
    bool ajoute_gauche(const TYPE&,noeud*&,iterator&);
    bool ajoute_droite(const TYPE&,noeud*&,iterator&);
    void reequilibrer(noeud*&);
    size_t getPoids(noeud*&);
    void rotation_gauche_droite(noeud*&);
    void rotation_droite_gauche(noeud*&);
    void afficher(WBT<TYPE>::noeud*,int,std::vector<std::string>&)const;
    void afficher_barres(std::vector<std::string>&,int)const;
public:
    WBT();
    ~WBT(){clear();}
    WBT(const WBT&);
    WBT& operator=(const WBT&);
    void swap(WBT&);
    
    size_t size()const{return RACINE.POIDS;}
    bool empty()const{return RACINE.POIDS==0;}
    void clear();
    
    iterator find(const TYPE&)const;
    std::pair<iterator,bool> insert(const TYPE&);
    size_t erase(const TYPE&);
    iterator erase(iterator);
    
    //fonction d'iteration
    iterator begin()const{return iterator(RACINE.DROITE);}
    iterator end()const{return iterator(FIN);}
    
    //fonction de mise au point
    void afficher()const;
};

template <typename TYPE>
class WBT<TYPE>::iterator{
private:
    noeud* POINTEUR;
    void avancer(noeud*&);
    void reculer(noeud*&);
    friend class WBT<TYPE>;
public:
    iterator(noeud*p=nullptr):POINTEUR(p){}
    iterator(const iterator&)=default;
    iterator& operator=(const iterator&)=default;
    const TYPE& operator*()const{return POINTEUR->CONTENU;}
    const TYPE* operator->()const{return *POINTEUR->CONTENU;}
    iterator& operator++(){avancer(POINTEUR);return *this;}                         //++i
    iterator operator++(int){iterator copie(*this);avancer(POINTEUR);return copie;} //i++
    iterator& operator--(){reculer(POINTEUR);return *this;}                         //--i
    iterator operator--(int){iterator copie(*this);reculer(POINTEUR);return copie;} //i--
    bool operator==(const iterator& dr)const{return POINTEUR==dr.POINTEUR;}
    bool operator!=(const iterator& dr)const{return POINTEUR!=dr.POINTEUR;}
};


///////////////////////////////////////////////////////////////////////////
// fonctions privees

template <typename TYPE>
void WBT<TYPE>::vider(noeud*& p){
    if(p==nullptr)return;
    vider(p->GAUCHE);
    vider(p->DROITE);
    delete p;
}

template <typename TYPE>
void WBT<TYPE>::copier(noeud* source,noeud*& dest,noeud* parent){
    if(source==nullptr)return;
    dest=new noeud(*source);
    dest->PARENT=parent;
    RACINE.DROITE=dest;
    copier(source->DROITE,dest->DROITE,dest);
    copier(source->GAUCHE,dest->GAUCHE,dest);
}


///////////////////////////////////////////////////////////////////////////
// insertion


template <typename TYPE>
bool WBT<TYPE>::insert(const TYPE& c,noeud*& p,iterator& r){
    if(c<p->CONTENU){
        if(ajoute_gauche(c,p,r)){
            ++(p->POIDS);
            reequilibrer(p);
            return true;
        }
    }
    else if(c>p->CONTENU){
        if(ajoute_droite(c,p,r)){
            ++(p->POIDS);
            reequilibrer(p);
            return true;
        }
    }
    else r=iterator(p);
    return false;
}

template <typename TYPE>
bool WBT<TYPE>::ajoute_gauche(const TYPE& c,noeud*& p,iterator& r){
    if(p->GAUCHE==nullptr){  //nouvelle feuille
        p->GAUCHE=new noeud(c,p);
        r=iterator(p->GAUCHE);
        if(p==RACINE.DROITE)RACINE.DROITE=p->GAUCHE;  //nouveau premier element
        return true;
    }
    else                    //ajout general a gauche
        return insert(c,p->GAUCHE,r);
}

template <typename TYPE>
bool WBT<TYPE>::ajoute_droite(const TYPE& c,noeud*& p,iterator& r){
    if(p->DROITE==nullptr){  //nouvelle feuille
        p->DROITE=new noeud(c,p);
        r=iterator(p->DROITE);
        return true;
    }
    else  //ajout general a droite
        return insert(c,p->DROITE,r);
}

///////////////////////////////////////////////////////////////////////////
// equilibre


template <typename TYPE>
void WBT<TYPE>::reequilibrer(noeud*& p){
    
    if (p==nullptr || p->POIDS < 3)
    {
        return;
    }
    
    if(getPoids(p->GAUCHE) *3 <= getPoids(p->DROITE)) {
        if (getPoids(p->GAUCHE) >= getPoids(p->DROITE->DROITE) && getPoids(p->GAUCHE) != 0) {
            rotation_gauche_droite(p->DROITE);
        }
        rotation_droite_gauche(p);
        
    } else if (getPoids(p->DROITE) *3 <= getPoids(p->GAUCHE)) {
        if (getPoids(p->DROITE) >= getPoids(p->GAUCHE->GAUCHE) && getPoids(p->DROITE)!= 0) {
            rotation_droite_gauche(p->GAUCHE);
        }
        rotation_gauche_droite(p);
    }
    
}

template <typename TYPE>
size_t WBT<TYPE>::getPoids(noeud*& p){
    
    size_t poids;
    if(p != nullptr) {
        poids = p->POIDS;
    } else {
        poids = 0;
    }
    
    return poids;
    
}

template <typename TYPE>
void WBT<TYPE>::rotation_gauche_droite(noeud*& p){
    
    noeud* temp = p->GAUCHE;
    
    if (temp->DROITE != nullptr)
        p->GAUCHE = temp->DROITE;
    else
        p->GAUCHE = nullptr;
    
    temp->DROITE = p;
    temp->PARENT = p->PARENT;
    
    p = temp->DROITE;
    p->PARENT = temp;
    
    p->POIDS = getPoids(p->DROITE) + 1;
    if (p->GAUCHE != nullptr) {
        p->POIDS += getPoids(p->GAUCHE);
    }
    
    temp->POIDS = getPoids(temp->DROITE) + 1;
    if (getPoids(temp->GAUCHE) != 0) {
        temp->POIDS += getPoids(temp->GAUCHE);
    }
    p = temp;
}

template <typename TYPE>
void WBT<TYPE>::rotation_droite_gauche(noeud*& p){
    
    noeud* temp = p->DROITE;
    
    if (temp->GAUCHE != nullptr)
        p->DROITE = temp->GAUCHE;
    else
        p->DROITE = nullptr;
    
    temp->GAUCHE = p;
    
    
    temp->GAUCHE = p;
    temp->PARENT = p->PARENT;
    
    p = temp->GAUCHE;
    
    
    
    p -> PARENT = temp;
    
    
    p->POIDS = getPoids(p->GAUCHE) + 1;
    if (p->DROITE != nullptr) {
        p->POIDS += p->DROITE->POIDS;
    }
    
    temp->POIDS = getPoids(temp->GAUCHE) + 1;
    if (getPoids(temp->DROITE) != 0) {
        temp->POIDS += getPoids(temp->DROITE);
    }
    p = temp;
}


///////////////////////////////////////////////////////////////////////////
// fonctions publiques

template <typename TYPE>
WBT<TYPE>::WBT(){
    FIN=&RACINE;
    RACINE.DROITE=FIN;
}

template <typename TYPE>
WBT<TYPE>::WBT(const WBT& source):WBT(){
    copier(source.FIN->GAUCHE,FIN->GAUCHE,FIN);
    RACINE.POIDS=source.size();
}

template <typename TYPE>
WBT<TYPE>& WBT<TYPE>::operator=(const WBT& source){
    WBT<TYPE> copie(source);
    this->swap(copie);
    return *this;
}

template <typename TYPE>
void WBT<TYPE>::swap(WBT& source){
    std::swap(FIN,source.FIN);
    std::swap(RACINE,source.RACINE);
}

template <typename TYPE>
void WBT<TYPE>::clear(){
    vider(RACINE.GAUCHE);
    RACINE.POIDS=0;
    RACINE.GAUCHE=nullptr;
    RACINE.DROITE=FIN;
}

template <typename TYPE>
typename WBT<TYPE>::iterator WBT<TYPE>::find(const TYPE& c)const{
    noeud* p=RACINE.GAUCHE;
    while(p!=nullptr)
        if(c<p->CONTENU)p=p->GAUCHE;
        else if(c>p->CONTENU)p=p->DROITE;
        else return iterator(p);
    return end();
}


template <typename TYPE>
std::pair<typename WBT<TYPE>::iterator,bool> WBT<TYPE>::insert(const TYPE& c){
    if(RACINE.POIDS==0){  //arbre vide
        RACINE.GAUCHE=RACINE.DROITE=new noeud(c,FIN);
        RACINE.POIDS=1;
        return std::make_pair(begin(),true);
    }
    else{
        iterator retour;
        if(insert(c,RACINE.GAUCHE,retour)){
            ++RACINE.POIDS;
            return std::make_pair(retour,true);
        }
        else return std::make_pair(retour,false);
    }
}

template <typename TYPE>
size_t WBT<TYPE>::erase(const TYPE& c){
    auto i=find(c);
    size_t resultat=1;
    if(i!=end())erase(i);
    else resultat=0;
    return resultat;
}

template <typename TYPE>
typename WBT<TYPE>::iterator WBT<TYPE>::erase(iterator i){
    noeud *p=i.POINTEUR,*temp,*enf,*pa;
    
    //preparer le resultat
    iterator resultat=i;
    ++resultat;
    if(i==begin())RACINE.DROITE=resultat.POINTEUR;
    
    //identifier l'element a enlever physiquement
    if(p->GAUCHE!=nullptr){
        for(temp=p->GAUCHE;temp->DROITE!=nullptr;)temp=temp->DROITE;
        p->CONTENU=temp->CONTENU;
    }
    else if(p->DROITE!=nullptr){
        for(temp=p->DROITE;temp->GAUCHE!=nullptr;)temp=temp->GAUCHE;
        p->CONTENU=temp->CONTENU;
    }
    else temp=p;
    
    pa=temp->PARENT;
    enf=temp->GAUCHE;
    if(enf==nullptr)enf=temp->DROITE;
    if(pa->GAUCHE==temp)pa->GAUCHE=enf;
    else pa->DROITE=enf;
    delete temp;
    
    //remonter pour mettre les poids a jour
    for(;;){
        --pa->POIDS;
        p=pa;
        pa=p->PARENT;
        if(pa==nullptr)break;
        if(pa->GAUCHE==p)reequilibrer(p->GAUCHE);
        else reequilibrer(p->DROITE);
    }
    
    return resultat;
}


///////////////////////////////////////////////////////////////////////////
// fonctions supplementaires de la class iterator

template <typename TYPE>
void WBT<TYPE>::iterator::avancer(noeud*& p){
    //AVANCER le pointeur p vers le prochain noeud en inordre
    assert(p->PARENT!=nullptr);
    if(p->DROITE!=nullptr)
        for(p=p->DROITE;p->GAUCHE!=nullptr;p=p->GAUCHE);
    else{
        noeud* pa;
        for(pa=p->PARENT;pa->GAUCHE!=p;p=pa,pa=p->PARENT);
        p=pa;
    }
}

template <typename TYPE>
void WBT<TYPE>::iterator::reculer(noeud*& p){
    //RECULER le pointeur p vers le noeud precedent en inordre
    if(p->GAUCHE!=nullptr)
        for(p=p->GAUCHE;p->DROITE!=nullptr;p=p->DROITE);
    else{
        noeud* pa;
        for(pa=p->PARENT;pa->DROITE!=p;p=pa,pa=p->PARENT);
        p=pa;
    }
    assert(p->PARENT!=nullptr);
    
}




///////////////////////////////////////////////////////////////////////////
// fonctions de mise au point
template <typename TYPE>
void WBT<TYPE>::afficher()const{
    using namespace std;
    size_t SIZE=RACINE.POIDS;
    cout<<"-------------------------------"<<endl;
    cout<<SIZE<<" element";if(SIZE>1)cout<<"s";cout<<endl;
    cout<<"racine: "<<RACINE.GAUCHE<<endl;
    cout<<"debut: "<<RACINE.DROITE<<endl;
    vector<string> barres;
    barres.push_back("    ");
    afficher(RACINE.GAUCHE,1,barres);
    cout<<"-------------------------------"<<endl;
}


template <typename TYPE>
void WBT<TYPE>::afficher(WBT<TYPE>::noeud* p,int niveau,std::vector<std::string>& barres)const{
    using namespace std;
    if(p==0)return;
    
    if(niveau>=barres.size())barres.push_back("    ");
    
    afficher(p->DROITE,niveau+1,barres);
    
    //si on est un enfant de gauche arreter les barres a ce niveau
    if(p->PARENT!=0 && p->PARENT->GAUCHE==p)barres[niveau-1]="    ";
    
    //cout<<niveau;
    afficher_barres(barres,niveau);
    cout<<"--->";
    cout<<p->CONTENU<<" ("<<p->POIDS;
    cout<<", "<<p;
    cout<<", par="<<p->PARENT;
    cout<<", gau="<<p->GAUCHE;;
    cout<<", dro="<<p->DROITE;
    cout<<")"<<endl;
    
    //si on est un enfant de droite barre a mon niveau
    if(p->PARENT!=0 && p->PARENT->DROITE==p)barres[niveau-1]="   |";
    
    //si on a un enfant a gauche mettre des barres apres
    if(p->GAUCHE!=0)barres[niveau]="   |";
    else barres[niveau]="    ";
    
    //cout<<niveau;
    afficher_barres(barres,niveau+1);
    cout<<endl;
    
    afficher(p->GAUCHE,niveau+1,barres);
}

template <typename TYPE>
void WBT<TYPE>::afficher_barres(std::vector<std::string>& barres,int n)const{
    for(int i=0;i<n;++i)std::cout<<barres[i];
}



#endif
