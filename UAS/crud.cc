#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "12345";
const char* dbname = "cpp_crud";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Berhasil terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

bool register_user(const string& username, const string& password, const string& role) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO users (username, password, role) VALUES ('" + username + "', '" + password + "', '" + role + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Registrasi gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        } else {
            cout << "Pengguna berhasil didaftarkan." << endl;
        }
        mysql_close(conn);
        return true;
    }
    return false;
}

bool login(const string& username, const string& password, string& role) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Query login gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            role = row[0];
            mysql_free_result(res);
            mysql_close(conn);
            return true;
        } else {
            mysql_free_result(res);
            mysql_close(conn);
            return false;
        }
    }
    return false;
}

void tambah_buku(const string& judul, const string& penulis, const string& genre, const string& penerbit, const string& tahun) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO books (judul, penulis, genre, penerbit, tahun) VALUES ('" + judul + "', '" + penulis + "', '" + genre + "', '" + penerbit + "', '" + tahun + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void lihat_buku() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM books")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Penulis: " << row[2] << ", Genre: " << row[3] << ", Penerbit: " << row[4] << ", Tahun: " << row[5] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void ubah_buku(int id_buku, const string& judul, const string& genre, const string& penerbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE books SET judul = '" << judul << "', genre = '" << genre << "', penerbit = '" << penerbit << "' WHERE id = " << id_buku;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diubah." << endl;
        }
        mysql_close(conn);
    }
}

void hapus_buku(int id_buku) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM books WHERE id = " << id_buku;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void lihat_pengguna() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM users")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Username: " << row[1] << ", Role: " << row[3] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void menu_admin() {
    int pilihan;
    while (true) {
        cout << "\nMenu Admin:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Lihat Semua Buku\n";
        cout << "3. Ubah Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Lihat Semua Pengguna\n";
        cout << "6. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            string judul, penulis, genre, penerbit, tahun;
            cout << "Masukkan judul: ";
            cin.ignore();
            getline(cin, judul);
            cout << "Masukkan penulis: ";
            getline(cin, penulis);
            cout << "Masukkan genre: ";
            getline(cin, genre);
            cout << "Masukkan penerbit: ";
            getline(cin, penerbit);
            cout << "Masukkan tahun: ";
            getline(cin, tahun);
            tambah_buku(judul, penulis, genre, penerbit, tahun);
        } else if (pilihan == 2) {
            lihat_buku();
        } else if (pilihan == 3) {
            int id_buku;
            string judul, genre, penerbit;
            cout << "Masukkan ID buku yang ingin diubah: ";
            cin >> id_buku;
            cin.ignore();
            cout << "Masukkan judul baru: ";
            getline(cin, judul);
            cout << "Masukkan genre baru: ";
            getline(cin, genre);
            cout << "Masukkan penerbit baru: ";
            getline(cin, penerbit);
            ubah_buku(id_buku, judul, genre, penerbit);
        } else if (pilihan == 4) {
            int id_buku;
            cout << "Masukkan ID buku yang ingin dihapus: ";
            cin >> id_buku;
            hapus_buku(id_buku);
        } else if (pilihan == 5) {
            lihat_pengguna();
        } else if (pilihan == 6) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

void menu_pengguna() {
    int pilihan;
    while (true) {
        cout << "\nMenu Pengguna:\n";
        cout << "1. Lihat Semua Buku\n";
        cout << "2. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            lihat_buku();
        } else if (pilihan == 2) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

void menu_utama() {
    int pilihan;
    while (true) {
        cout << "\nMenu Utama:\n";
        cout << "1. Daftar\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            string username, password, role;
            cout << "Masukkan username: ";
            cin >> username;
            cout << "Masukkan password: ";
            cin >> password;
            cout << "Masukkan peran (admin/pengguna): ";
            cin >> role;
            if (role == "admin" || role == "pengguna") {
                register_user(username, password, role);
            } else {
                cout << "Peran tidak valid. Silakan masukkan 'admin' atau 'pengguna'." << endl;
            }
        } else if (pilihan == 2) {
            string username, password, role;
            cout << "Masukkan username: ";
            cin >> username;
            cout << "Masukkan password: ";
            cin >> password;

            if (login(username, password, role)) {
                if (role == "admin") {
                    menu_admin();
                } else if (role == "pengguna") {
                    menu_pengguna();
                } else {
                    cerr << "Peran tidak valid. Keluar..." << endl;
                }
            } else {
                cerr << "Login gagal. Silakan coba lagi." << endl;
            }
        } else if (pilihan == 3) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

int main() {
    menu_utama();
    return 0;
}
