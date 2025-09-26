/* Anggota Kelompok:
1. Muhammad Ershad Hanif Radhiyya (245150301111015)
2. Muhammad Banyubiru Faiq (245150307111027)
3. Muhammad Aushaf Farras (245150307111021)
4. Arief Musa Bintang (225150300111038)
*/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

class Barang {
private:
    string id;
    string nama;
    int stok;

public:
    Barang(string i = "", string n = "", int s = 0) : id(i), nama(n), stok(s) {} 

    string getID() const { return id; }
    string getNama() const { return nama; }
    int getStok() const { return stok; }

    bool setNama(string newNama){
        if (!newNama.empty()){
            nama = newNama;
            return true;
        }
        return false;
    }

    bool setStok(int newStok){
        if (newStok >= 0){
            stok = newStok;
            return true;
        }
        cout << "Stok Tidak Boleh Negatif!\n";
        return false;
    }

    bool addStok(int jumlah){
        return setStok(stok + jumlah);
    }

    bool decreaseStok (int jumlah){
        if (stok >= jumlah){
            return setStok(stok - jumlah);
        }
        cout << "Stok Tidak Mencukupi!\n";
    }

    void display() const {
        cout << "Nama: " << nama
             << " | ID: " << id 
             << " | Stok: " << stok << endl;
    }
};

class BST {
private:
    struct Node {
        Barang data;
        Node* left;
        Node* right;

        Node(Barang b) : data(b), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void inorderRec(Node* node) const {
        if (node){
            inorderRec(node->left);
            node->data.display();
            inorderRec(node->right);
        }
    }

    void collectAllItems(Node* node, vector<Barang>& items) {
        if (!node) return;
        collectAllItems(node->left, items);
        items.push_back(node->data);
        collectAllItems(node->right, items);
    }

    void sortItems(vector<Barang>& items, const string& sortBy) {
        for (int i = 1; i < items.size(); i++) {
            Barang key = items[i];
            int j = i - 1;
            
            while (j >= 0) {
                bool shouldSwap = false;
                
                if (sortBy == "nama") {
                    string nama1 = items[j].getNama();
                    string nama2 = key.getNama();
                    transform(nama1.begin(), nama1.end(), nama1.begin(), ::tolower);
                    transform(nama2.begin(), nama2.end(), nama2.begin(), ::tolower);
                    shouldSwap = nama1 > nama2;
                }
                else if (sortBy == "stok") {
                    shouldSwap = items[j].getStok() > key.getStok();
                }
                else { 
                    shouldSwap = items[j].getID() > key.getID();
                }
                
                if (shouldSwap) {
                    items[j + 1] = items[j];
                    j--;
                } else {
                    break;
                }
            }
            items[j + 1] = key;
        }
    }

    Node* insertRec(Node* node, Barang barang){
        if(!node) return new Node(barang);

        if(barang.getID() < node->data.getID()){
            node->left = insertRec(node->left, barang);
        } else if (barang.getID() > node->data.getID()){
            node->right = insertRec(node->right, barang);
        }
        return node;
    }

    Node* findMin(Node* node){
        while (node && node->left) node = node->left;
        return node;
    }

    Node* deleteRec(Node* node, string id){
        if (!node) return nullptr;

        if (id < node->data.getID()){
            node->left = deleteRec(node->left, id);
        } else if (id > node->data.getID()){
            node->right = deleteRec(node->right, id);
        } else {
            if (!node->left){
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right){
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteRec(node->right, temp->data.getID());
        }
        return node;
    }

    Node* searchRec(Node* node, string id){
        if (!node || node->data.getID() == id) return node;
        if (id < node->data.getID()) return searchRec(node->left, id);

        return searchRec(node->right, id);
    }

    void searchByNamaRec(Node* node, string keyword){
        if (node){
            searchByNamaRec(node->left, keyword);
            string nama = node->data.getNama();
            transform(nama.begin(), nama.end(), nama.begin(), ::tolower);
            if (nama.find(keyword) != string::npos){
                node->data.display();
            }
            searchByNamaRec(node->right, keyword);
        }
    }

public:
    BST() : root(nullptr) {}

    void displayBarang() const {
        if (!root){
            cout << "Inventori Kosong!\n";
            return;
        }
        cout << "\n===== DAFTAR BARANG =====\n";
        inorderRec(root);
        cout << "==========================\n";
    }

    void displayTerurut(const string& sortBy = "id") {
        if (!root) {
            cout << "Inventori Kosong!\n";
            return;
        }
        
        vector<Barang> items;
        collectAllItems(root, items);
        sortItems(items, sortBy);

        cout << "\n===== DAFTAR BARANG (TERURUT";
        if (sortBy == "nama") cout << " BERDASARKAN NAMA) ";
        else if (sortBy == "stok") cout << " BERDASARKAN STOK) ";
        else cout << " BERDASARKAN ID) ==";
        cout << "=====\n";
        
        for (const Barang& b : items) {
            b.display();
        }
        cout << "====================================================\n";
    }

    void addBarang(Barang barang){
        root = insertRec(root, barang);
        cout << "\nBarang Berhasil Ditambahkan.\n";
    }

    bool updateBarang(string id){
        Node* node = searchRec(root, id);
        if(!node){
            cout <<"Barang Tidak Ditemukan!\n";
            return false;
        }

        cout << "\nBarang ditemukan:\n";
        node->data.display();

        string newNama;
        int newStok;
        cout << "\nMasukkan nama baru (kosongkan jika tidak berubah): ";
        getline(cin, newNama);

        if(!newNama.empty()) node->data.setNama(newNama);

        cout << "Masukkan stok baru (-1 jika tidak berubah): ";
        cin >> newStok;
        cin.ignore();

        if(newStok != -1) node->data.setStok(newStok);

        cout << "\nUpdate Berhasil.\n";
        return true;
    }

    bool deleteBarang(string id){
        if(!searchRec(root, id)){
            cout << "Barang Tidak Ditemukan!\n";
            return false;
        }
        root = deleteRec(root, id);
        cout << "\nBarang Berhasil Dihapus.\n";
        return true;
    }

    void searchByID(string id){
        Node* result = searchRec(root, id);
        if (result){
            cout << "\nHasil pencarian:\n";
            result->data.display();
        } else {
            cout << "Barang Tidak Ditemukan!\n";
        }
    }

    void searchByNama(string keyword){
        transform(keyword.begin(), keyword.end(), keyword.begin(), :: tolower);
        cout << "\nHasil Pencarian '" << keyword << "':\n";
        searchByNamaRec(root, keyword);
    }

};

void displayMenu(){
    cout << "\n===== SELAMAT  DATANG   DI  ======"
         << "\n===== SISTEM MANAJEMEN STOK ======"
         << "\n=====        SIGMAMART      ======"
         << "\nPilihan Menu:"
         << "\n1. Tampilkan Semua Barang (Inorder)"
         << "\n2. Tampilkan Semua Barang (Terurut)"
         << "\n3. Tambah Barang "
         << "\n4. Update Barang"
         << "\n5. Hapus Barang"
         << "\n6. Cari Berdasarkan ID"
         << "\n7. Cari Berdasarkan Nama"
         << "\n8. Keluar"
         << "\nSilahkan Input Pilihan: ";
}

int main(){
    BST inventori;
    int pilihan;
    string inputID, inputNama;
    int inputStok;

    do {
        displayMenu();
        cin >> pilihan;
        cin.ignore();

        switch(pilihan){
            case 1:
                inventori.displayBarang();
                break;
            case 2: {
                string sortOption;
                cout << "\nUrutkan berdasarkan (id/nama/stok): ";
                getline(cin, sortOption);
    
                if (sortOption != "nama" && sortOption != "stok") {
                    sortOption = "id"; 
                }
                inventori.displayTerurut(sortOption);
                break;
            }
            case 3:
                cout << "\nTambah Barang Baru"
                     << "\nID Barang: ";
                getline(cin, inputID);
                cout << "Nama Barang: ";
                getline(cin, inputNama);
                cout << "Stok Awal: ";
                cin >> inputStok;
                cin.ignore();
                inventori.addBarang(Barang(inputID, inputNama, inputStok));
                break;
            case 4:
                cout << "Masukkan ID Barang: ";
                getline(cin, inputID);
                inventori.updateBarang(inputID);
                break;
            case 5:
                cout << "Masukkan ID Barang: ";
                getline(cin, inputID);
                inventori.deleteBarang(inputID);
                break;
            case 6:
                cout << "Masukkan ID Barang: ";
                getline(cin, inputID);
                inventori.searchByID(inputID);
                break;
            case 7: 
                cout << "Masukkan Nama Barang: ";
                getline(cin, inputNama);
                inventori.searchByNama(inputNama);
                break;
            case 8:
                cout << "\nTerima Kasih Telah Menggunakan Sistem!"
                     << "\nMengeluarkan dari Sistem...";
                break;
            default:
                cout << "Pilihan Tidak Valid!\n";
        }
        if (pilihan != 8){
            cout << "\nTekan ENTER Untuk Melanjutkan.";
            cin.ignore();
        }
    } while (pilihan!= 8);

    return 0;
}
