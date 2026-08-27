#include "../hotel.h"

vector<Customer> customers;
string currentLoggedInCustomer = "";

// This list is the start of every run. Bookings stay in memory only.
vector<Room> roomList = {
	{"101", "Single",        1, 120.00, "Available"},
	{"102", "Single",        1, 120.00, "Available"},
	{"103", "Single",        1, 120.00, "Available"},
	{"104", "Single",        1, 120.00, "Available"},
	{"105", "Single",        1, 120.00, "Cleaning"},
	{"201", "Twin",          2, 150.00, "Available"},
	{"202", "Twin",          2, 150.00, "Available"},
	{"203", "Twin",          2, 150.00, "Available"},
	{"204", "Twin",          2, 150.00, "Available"},
	{"301", "Deluxe",        2, 180.00, "Available"},
	{"302", "Deluxe",        2, 180.00, "Available"},
	{"303", "Deluxe",        2, 180.00, "Available"},
	{"304", "Deluxe",        2, 180.00, "Available"},
	{"305", "Deluxe",        2, 180.00, "Cleaning"},
	{"401", "Family",        4, 250.00, "Available"},
	{"402", "Family",        4, 250.00, "Available"},
	{"403", "Family",        4, 250.00, "Available"},
	{"501", "Suite",         4, 320.00, "Available"},
	{"502", "Suite",         4, 320.00, "Available"},
	{"503", "Suite",         4, 320.00, "Available"},
	{"504", "Suite",         4, 320.00, "Maintenance"},
	{"601", "Presidential",  6, 580.00, "Available"},
	{"602", "Presidential",  6, 580.00, "Available"}
};

vector<BookingRecord> reservations;
int nextReservationID = 1;
int nextInvoiceNo = 1;

vector<string> currentSessionIDs;
string sessionPromoCode = "";
double sessionPromoPercent = 0.0;
double sessionPromoFlat = 0.0;
int sessionRedeemedPoints = 0;
bool sessionSurpriseGiven = false;

// Real hotels by state and area. Every hotel uses the same roomList.
static const char* const HOTEL_ROWS[][4] = {
	{"Perlis", "Kangar", "Ants Hotel", "No. 12, Jalan Bintong Mewah 1, Bintong Mewah, 01000 Kangar, Perlis"},
	{"Perlis", "Kangar", "Hotel Federal Kangar", "Kampung Tok Peduka, 01000 Kangar, Perlis"},
	{"Perlis", "Kangar", "Hotel Seri Malaysia Kangar", "LOT 8101 MUKIM, MALAYS, Persiaran Wawasan, Taman Budaya, 01000 Kangar, Perlis"},
	{"Perlis", "Kangar", "The Putra Regency Hotel", "135, Persiaran Jubli Emas, 01000 Kangar, Perlis"},
	{"Perlis", "Kuala Perlis", "Putra Brasmana Hotel", "Jalan Kampung Perak, Pekan Kuala Perlis, 02000 Kuala Perlis, Perlis"},
	{"Perlis", "Kuala Perlis", "Savana Hotel Kuala Perlis", "No. 1,3,5, Blok C, KOMPLEKS PERNIAGAAN, Jalan Empat, Pekan Kuala Perlis, 02000 Kuala Perlis, Perlis"},
	{"Perlis", "Kuala Perlis", "T Hotel Kuala Perlis", "1, Persiaran Putra Timur, Pekan Kuala Perlis, 02000 Kuala Perlis, Perlis"},
	{"Perlis", "Arau", "All in Hotel", "NO. 33, Persiaran Jejawi Utara Kompleks Perniagaan, Kampung Jejawi, 02600 Arau, Perlis"},
	{"Perlis", "Arau", "Hotel Metro Inn Arau", "No. 27-35, Jalan Dua, Kompleks Arau, 02600 Arau, Perlis"},
	{"Perlis", "Arau", "Rumah Tetamu Arau (Royal Guest House)", "32, Lorong 1 Taman Seri Mawar, Arau, 02600 Arau, Perlis"},
	{"Perlis", "Padang Besar", "Padang Besar Green Inn", "No 82, Kampung Melayu, 02100 Padang Besar, Perlis"},
	{"Perlis", "Padang Besar", "SBS Hotel", "12, Jalan Singgahsana 5, Taman Singgahsana, 02100 Padang Besar, Perlis"},
	{"Perlis", "Beseri", "ClubRock Perlis Signature Hotel", "Gua Kelam, Kaki Bukit, 02200 Kaki Bukit, Perlis"},
	{"Perlis", "Beseri", "Hotel O Panggas Village", "Lot 8048, Kampung Panggas Besar, 02500 Kangar, Perlis"},
	{"Perlis", "Beseri", "Tasoh Lake Resort", "Lot 3082, Jalan, Kampung Buit Manek, 02200 Kaki Bukit, Perlis"},
	{"Perlis", "Beseri", "Terinai Lake View Resort", "Jalan Kaki Bukit, 02400 Kangar, Perlis"},
	{"Perlis", "Pauh", "Vilana Hotel Pauh", "1, Jalan Lapan Belas, Taman Pauh Indah, 02600 Arau, Perlis"},
	{"Kedah", "Kota Setar", "Hotel Fuller Alor Setar", "Unit 1, Kompleks Perniagaan Pintu 10, Jalan Pintu Sepuluh, Kampung Lubok Peringgi, 05100 Alor Setar, Kedah"},
	{"Kedah", "Kota Setar", "Hotel Seri Malaysia Alor Setar", "Jalan Stadium, City Center, 05150 Alor Setar, Kedah"},
	{"Kedah", "Kota Setar", "Royale Signature Hotel", "Jalan Lumpur, Kampung Lubok Peringgi, 05150 Alor Setar, Kedah"},
	{"Kedah", "Kota Setar", "The Grand Alora Hotel", "No. 888 Persiaran Bandar Baru Mergong, Lebuhraya Sultanah Bahiyah, 05150 Alor Setar, Kedah"},
	{"Kedah", "Langkawi", "Adya Hotel Langkawi", "No 1 PT 4001 Mukim Darul Aman Persiaran Mutiara 2 Mukim Kuah, 07000 Langkawi, Kedah"},
	{"Kedah", "Langkawi", "The Pelangi Beach Resort & Spa", "Pantai Cenang, 07000 Langkawi, Kedah"},
	{"Kedah", "Langkawi", "The Ritz-Carlton, Langkawi", "PT 313, Jalan Pantai Kok, Teluk Nibung, 07000 Langkawi, Kedah"},
	{"Kedah", "Kuala Muda", "Emerald Puteri Hotel", "50, Jalan Permatang Gedong, Taman Sejati Indah, 08000 Sungai Petani, Kedah"},
	{"Kedah", "Kuala Muda", "Swiss-Inn Sungai Petani", "1A, Jalan Pahlawan, 08000 Sungai Petani, Kedah"},
	{"Kedah", "Kuala Muda", "The Pure Hotel Sungai Petani", "No A2, 1, Jalan Indah 2, Taman Sejati Indah, 08000 Sungai Petani, Kedah"},
	{"Kedah", "Kulim", "E Hotel", "190, Jalan Seluang, Taman Seluang, 09000 Kulim, Kedah"},
	{"Kedah", "Kulim", "Fuller Hotel", "50, Jalan Kota Kenari 1, Taman Kenari, 09000 Kulim, Kedah"},
	{"Kedah", "Kulim", "Hotel Seri Malaysia Kulim", "No 2, Taman Tasik Putra, 09000 Kulim, Kedah"},
	{"Kedah", "Kubang Pasu", "EDC Hotel & Resorts UUM", "Universiti Utara Malaysia, Sintok, 06010 Bukit Kayu Hitam, Kedah"},
	{"Kedah", "Kubang Pasu", "Hotel Darulaman Jitra", "Bandar Darulaman, 06000 Jitra, Kedah"},
	{"Kedah", "Kubang Pasu", "Stay 365 Hotel", "55, Jalan Teja 2, Taman Teja, 06010 Changlun, Kedah"},
	{"Kedah", "Yan", "The Jerai Hill Resort", "Jalan Kaki Bukit Lebuhraya Utara - Selatan, 08300 Gurun, Kedah"},
	{"Pulau Pinang", "George Town", "Cheong Fatt Tze - Blue Mansion", "14, Lebuh Leith, 10200 George Town, Pulau Pinang"},
	{"Pulau Pinang", "George Town", "Eastern & Oriental (E&O) Hotel", "10, Lebuh Farquhar, 10200 George Town, Pulau Pinang"},
	{"Pulau Pinang", "George Town", "Seven Terraces", "14A, Stewart Lane, 10200 George Town, Pulau Pinang"},
	{"Pulau Pinang", "Batu Ferringhi", "Hard Rock Hotel Penang", "Batu Ferringhi Beach, 11100 Batu Ferringhi, Pulau Pinang"},
	{"Pulau Pinang", "Batu Ferringhi", "Shangri-La Rasa Sayang", "Jalan Batu Ferringhi, 11100 Batu Ferringhi, Pulau Pinang"},
	{"Pulau Pinang", "Gurney Drive", "G Hotel Gurney", "168A, Persiaran Gurney, 10250 George Town, Pulau Pinang"},
	{"Pulau Pinang", "Gurney Drive", "The Millen Penang, Autograph Collection", "55, Jln Sultan Ahmad Shah, 10050 George Town, Pulau Pinang"},
	{"Pulau Pinang", "Tanjung Bungah", "Hompton by the Beach Penang", "533, Jalan C M Hashim, Tanjung Tokong, 11200 Tanjung Bungah, Pulau Pinang"},
	{"Pulau Pinang", "Tanjung Bungah", "Mercure Penang Beach", "505, Jalan Tanjung Bungah, 11200 Tanjung Bungah, Pulau Pinang"},
	{"Pulau Pinang", "Bayan Lepas", "AC Hotel by Marriott Penang", "213, Jalan Bukit Gambir, Kampung Seberang Paya, 11950 Bayan Lepas, Pulau Pinang"},
	{"Pulau Pinang", "Bayan Lepas", "Amari SPICE Penang", "2, Persiaran Mahsuri, Bayan Baru, 11900 Bayan Lepas, Pulau Pinang"},
	{"Pulau Pinang", "Air Itam", "Bellevue The Penang Hill Hotel", "Jalan Bukit Bendera, Jalan Stesen, 11500 George Town, Pulau Pinang"},
	{"Pulau Pinang", "Butterworth", "The Light Hotel Penang", "Lebuh Tenggiri 2, 13700 Seberang Jaya, Pulau Pinang"},
	{"Pulau Pinang", "Bukit Mertajam", "Vangohh Eminent Hotel & Spa", "3406, Jalan Machang Bubok 2, Taman Machang Bubok, 14000 Bukit Mertajam, Pulau Pinang"},
	{"Perak", "Kinta", "The Banjaran Hotsprings Retreat Ipoh", "1, Persiaran Lagoon, Sunway 3, 31150 Ipoh, Perak"},
	{"Perak", "Kinta", "The Haven All Suite Resort Ipoh", "Persiaran Lembah Perpaduan, 31150 Ipoh, Perak"},
	{"Perak", "Kinta", "WEIL Hotel Ipoh", "292, Jalan Sultan Idris Shah, 30000 Ipoh, Perak"},
	{"Perak", "Larut, Matang & Selama", "Flemington Hotel Taiping", "1, Jalan Samanea Saman, 34000 Taiping, Perak"},
	{"Perak", "Larut, Matang & Selama", "Grand Baron Hotel Taiping", "34000 Taiping, Perak"},
	{"Perak", "Larut, Matang & Selama", "Novotel Taiping", "1, Jalan Tupai, 34000 Taiping, Perak"},
	{"Perak", "Manjung", "Marina Island Pangkor Resort & Hotel", "Block Laguna 2, KM 1, Jalan Utama Marina Island, Teluk Muroh, 32200 Lumut, Perak"},
	{"Perak", "Manjung", "Pangkor Laut Resort", "Pangkor Laut Island, 32200 Lumut, Perak"},
	{"Perak", "Manjung", "Swiss-Garden Beach Resort Damai Laut", "Persiaran Swiss-Garden, Jalan Damai Laut, 32200 Lumut, Perak"},
	{"Perak", "Hilir Perak", "Anson Hotel Teluk Intan", "Jalan Sekolah, Pekan Teluk Intan, 36000 Teluk Intan, Perak"},
	{"Perak", "Hilir Perak", "Grand Court Hotel Teluk Intan", "G-3, Menara GSM, Jalan Padang Tembak, 36000 Teluk Intan, Perak"},
	{"Perak", "Kuala Kangsar", "Hotel Seri Malaysia Kuala Kangsar", "33, Jalan Daeng Selili, Kampung Penaga, 33000 Kuala Kangsar, Perak"},
	{"Perak", "Kuala Kangsar", "The Shop Hotel Kuala Kangsar", "1, Persiaran Seri Delima, Taman Seri Delima, 33000 Kuala Kangsar, Perak"},
	{"Perak", "Muallim", "Sg Perak Resort Hotel", "56A-B, Jalan Bunga Raya, 35900 Tanjong Malim, Perak"},
	{"Perak", "Kampar", "Grand Kampar Hotel", "2188, Jalan Timah, Bandar Baru, 31900 Kampar, Perak"},
	{"Perak", "Kampar", "The Trails Of Kampar", "Jln Persiaran Residensi, 31900 Kampar, Perak"},
	{"Perak", "Hulu Perak", "Belum Rainforest Resort", "Lebuhraya Timur-Barat, Pulau Banding, 33300 Gerik, Perak"},
	{"Perak", "Kerian", "Damai Hotel Parit Buntar", "Lot 6298, Jln Taiping, 34200 Parit Buntar, Perak"},
	{"Selangor", "Petaling Jaya", "Le Meridien Petaling Jaya", "1, Jalan SS 7/26a, SS 7, 47301 Petaling Jaya, Selangor"},
	{"Selangor", "Petaling Jaya", "One World Hotel", "City Centre, First Avenue, Lebuh Bandar Utama, Bandar Utama, 47800 Petaling Jaya, Selangor"},
	{"Selangor", "Petaling Jaya", "Sunway Resort Hotel", "1, Persiaran Lagoon, Bandar Sunway, 47500 Petaling Jaya, Selangor"},
	{"Selangor", "Hulu Langat", "Bangi Resort Hotel", "Off Persiaran Bandar, Bangi Golf Club, 43650 Bandar Baru Bangi, Selangor"},
	{"Selangor", "Hulu Langat", "Grand Barakah Hotel", "Jalan Selaman 1, Palm Square, 68000 Ampang Jaya, Selangor"},
	{"Selangor", "Hulu Langat", "RHR Hotel", "Wisma MKH, Bandar Kajang, 43000 Kajang, Selangor"},
	{"Selangor", "Klang", "Crystal Crown Hotel Harbour View Klang", "217, Persiaran Raja Muda Musa, 42000 Pelabuhan Klang, Selangor"},
	{"Selangor", "Klang", "Premiere Hotel Klang", "Bandar Bukit Tinggi 1/KS6, Jalan Langat, 41200 Klang, Selangor"},
	{"Selangor", "Klang", "Wyndham Acentria Klang Hotel", "1-G-1, Persiaran Bukit Raja 2, KU1, 41150 Klang, Selangor"},
	{"Selangor", "Gombak", "Mercure Selangor Selayang", "No B G 12, Dataran Emerald, Jalan Ps 11, Prima Selayang, 68100 Batu Caves, Selangor"},
	{"Selangor", "Sepang", "Cyberview Resort & Spa", "The Lodge, Persiaran Multimedia, Cyber 7, 63000 Cyberjaya, Selangor"},
	{"Selangor", "Sepang", "Movenpick Hotel & Convention Centre KLIA", "Kompleks TH Sepang, Jalan Masjid KLIA, 64000 Sepang, Selangor"},
	{"Selangor", "Sepang", "Sama-sama Hotel KLIA", "90, Jln CTA 4B, Kuala Lumpur International Airport, 64000 Sepang, Selangor"},
	{"Selangor", "Kuala Langat", "Amverton Cove Golf & Island Resort", "Jalan Pulau Carey, 42960 Pulau Carey, Selangor"},
	{"Selangor", "Kuala Langat", "Gold Coast Morib International Resort", "PT 294, Morib Beach, 42700 Banting, Selangor"},
	{"Selangor", "Kuala Selangor", "De Palma Resort Kuala Selangor", "Jalan Tanjung Keramat, 45000 Kuala Selangor, Selangor"},
	{"Selangor", "Kuala Selangor", "Padi Box Eco Resort", "Jalan Tali Air 4, Kampung Parit Empat, 45400 Sekinchan, Selangor"},
	{"Selangor", "Sabak Bernam", "Grand Court Hotel Sabak Bernam", "1, Jalan 2, Taman Berkat, 45300 Sungai Besar, Selangor"},
	{"Kuala Lumpur", "KLCC", "Hotel Maya Kuala Lumpur", "138, Jln Ampang, 50450 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "KLCC", "Impiana KLCC Hotel", "13, Jalan Pinang, 50450 Kuala Lumpur"},
	{"Kuala Lumpur", "KLCC", "Santa Grand Signature Kuala Lumpur", "138, Jln Ampang, 50450 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Bukit Bintang", "ANSAR Hotel Kuala Lumpur", "101, Jln Bukit Bintang, Bukit Bintang, 55100 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Bukit Bintang", "Izumi Hotel Bukit Bintang", "85, Jalan Berangan, Off, Jln Bukit Bintang, Bukit Bintang, 50200 Kuala Lumpur"},
	{"Kuala Lumpur", "Bukit Bintang", "Pavilion Hotel Kuala Lumpur", "170, Jln Bukit Bintang, Bukit Bintang, 55100 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Bukit Bintang", "WOLO Kuala Lumpur", "Corner of Jalan Bukit Bintang, Jln Sultan Ismail, 55100 Kuala Lumpur"},
	{"Kuala Lumpur", "Chinatown", "Else Kuala Lumpur", "145, Jalan Tun H S Lee, City Centre, 50000 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Chinatown", "Four Points by Sheraton Kuala Lumpur, Chinatown", "2, Jalan Balai Polis, City Centre, 50000 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Chinatown", "Space Hotel Chinatown", "5, Jalan Petaling, City Centre, 50000 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "KL Sentral", "Easy Hotel KL Sentral", "110, JALAN TUN SAMBANTHAN BRICKFIELD, Kuala Lumpur Sentral, 50470 Wilayah Persekutuan, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "KL Sentral", "Hotel Sentral Kuala Lumpur", "30, Jalan Thambipillay, Brickfields, 50470 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "KL Sentral", "Le Meridien Kuala Lumpur", "2, Jalan Stesen Sentral, KL Sentral, 50470 Kuala Lumpur"},
	{"Kuala Lumpur", "KL Sentral", "The St. Regis Kuala Lumpur", "6, Jalan Stesen Sentral 2, Kuala Lumpur Sentral, 50470 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Bangsar / Mont Kiara", "Amari Kuala Lumpur", "3, Jalan Bangsar, KL Eco City, 59200 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Bangsar / Mont Kiara", "Hyatt House Kuala Lumpur, Mont Kiara", "G-2 Arcoris, 10, Jalan Kiara, Mont Kiara, 50480 Kuala Lumpur"},
	{"Kuala Lumpur", "Bangsar / Mont Kiara", "The Signature Hotel & Serviced Suites Hartamas", "3, Jln Sri Hartamas 22, Desa Sri Hartamas, 50480 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Mid Valley City", "St Giles Mid Valley Kuala Lumpur", "The Gardens, Mid Valley City, Lingkaran Syed Putra, 59200 Kuala Lumpur"},
	{"Kuala Lumpur", "Kepong", "Crystal Crown Hotel Kuala Lumpur (Kepong)", "3, Jalan Jambu Mawar, Off, Jln Kepong, 52000 Kuala Lumpur"},
	{"Kuala Lumpur", "Kepong", "KIP Hotel Kuala Lumpur", "77, Jalan Seri Utara 1, Sri Utara Business Park, 68100 Kuala Lumpur, Selangor"},
	{"Kuala Lumpur", "Cheras", "Hotel Caliber", "46-56, Jalan 5/101c, Cheras Business Centre, 56100 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Kuala Lumpur", "Cheras", "Silka Cheras Kuala Lumpur", "100, Jln Cheras, Phoenix Business Park, 56000 Kuala Lumpur, Wilayah Persekutuan Kuala Lumpur"},
	{"Negeri Sembilan", "Port Dickson", "Avillion Port Dickson", "3rd Mile, Jalan Pantai, 71000 Port Dickson, Negeri Sembilan"},
	{"Negeri Sembilan", "Port Dickson", "Grand Lexis Port Dickson", "Batu 2, Jalan Seremban, 71000 Port Dickson, Negeri Sembilan"},
	{"Negeri Sembilan", "Port Dickson", "Lexis Hibiscus Port Dickson", "12th Mile, Jalan Pantai, 71250 Port Dickson, Negeri Sembilan"},
	{"Negeri Sembilan", "Port Dickson", "Thistle Port Dickson Resort", "KM16, Jalan Pantai Teluk Kemang, 71050 Port Dickson, Negeri Sembilan"},
	{"Negeri Sembilan", "Seremban", "Palm Seremban Hotel", "Jalan Toman 1, Kemayan Square, 70200 Seremban, Negeri Sembilan"},
	{"Negeri Sembilan", "Seremban", "Royale Chulan Seremban", "Jalan Dato A.S Dawood, 70100 Seremban, Negeri Sembilan"},
	{"Negeri Sembilan", "Nilai", "Nilai Springs Resort Hotel", "PT 4770, Bandar Baru Nilai, 71800 Nilai, Negeri Sembilan"},
	{"Negeri Sembilan", "Nilai", "Sri Enstek Hotel", "61, Jln Timur 6/1B, Bandar Baru Enstek, 71800 Bandar Baru Enstek, Negeri Sembilan"},
	{"Negeri Sembilan", "Kuala Pilah", "Melang Inn", "PT 1, Jalan Melang, Taman Pilah Jaya, 72000 Kuala Pilah, Negeri Sembilan"},
	{"Negeri Sembilan", "Jelebu", "D'Inara @ Jelebu", "Lot 1830, Batu 19 1/2, Kampung Gedang, 71600 Kuala Klawang, Negeri Sembilan"},
	{"Negeri Sembilan", "Rembau", "Seri Rembau Hotel", "No. 31, Jalan RS2, Rembau Sentral, 71300 Rembau, Negeri Sembilan"},
	{"Negeri Sembilan", "Lukut", "Tasik Villa International Resort", "Jalan Tasik Villa, Kota Lukut, 71010 Port Dickson, Negeri Sembilan"},
	{"Negeri Sembilan", "Rantau", "OYO 44010 Royals Star Hotel", "Jalan Tuanku Antah, 70100 Seremban, Negeri Sembilan"},
	{"Melaka", "Historic Core", "Liu Men Hotel Melaka", "46-56, Jalan Tokong, Kampung Dua, 75200 Melaka"},
	{"Melaka", "Historic Core", "Quayside Hotel Melaka", "Jln Merdeka, Banda Hilir, 75000 Melaka"},
	{"Melaka", "Historic Core", "TheBlanc Boutique Hotel", "26-28, Jalan Kubu, 75300 Melaka"},
	{"Melaka", "Banda Hilir", "Estadia Hotel", "Jln Merdeka, Banda Hilir, 75000 Melaka"},
	{"Melaka", "Banda Hilir", "Hatten Hotel Melaka", "Jln Merdeka, Banda Hilir, 75000 Melaka"},
	{"Melaka", "Banda Hilir", "Holiday Inn Melaka", "Jln Syed Abdul Aziz, 75000 Melaka"},
	{"Melaka", "Melaka River Front", "Swiss Garden Hotel Melaka", "T2-4, The Shore @ Melaka River, Jalan Persisiran Bunga Raya, 75300 Melaka"},
	{"Melaka", "Melaka River Front", "The Shore Hotel & Residences", "Level GF, 193, Pinggiran Sungai Melaka, Jalan Persisiran Bunga Raya, 75100 Melaka"},
	{"Melaka", "Kota Laksamana", "Asteria Melaka by Attic Suite", "Jalan KSB 11A, 75200 Melaka"},
	{"Melaka", "Kota Laksamana", "The Stable Melaka Guest House", "Jalan Hang Kasturi, Taman Kota Laksamana, 75200 Melaka"},
	{"Melaka", "Alor Gajah", "A'Famosa Resort", "Jalan Kemus, Simpang Ampat, 78000 Alor Gajah, Melaka"},
	{"Melaka", "Ayer Keroh", "Ames Hotel", "Jalan Pkak 2, Pusat Komersial, 75450 Ayer Keroh, Melaka"},
	{"Melaka", "Pulau Melaka", "Birkin International Hotel", "LOT 12191, BANDAR XLV, 75200 Malacca"},
	{"Johor", "Johor Bahru", "Amari Johor Bahru", "82C, Jalan Trus, Bandar Johor Bahru, 80000 Johor Bahru, Johor Darul Ta'zim"},
	{"Johor", "Johor Bahru", "Grand Paragon Hotel", "18, Jalan Harimau, Taman Century, 80250 Johor Bahru, Johor Darul Ta'zim"},
	{"Johor", "Johor Bahru", "Holiday Inn Johor Bahru City Centre", "Jalan Tun Abdul Razak, 80000 Johor Bahru, Johor"},
	{"Johor", "Iskandar Puteri", "Ibis Styles Johor Iskandar Puteri", "1 Nusa Sentrum Complex, Jalan SILC, 79200 Iskandar Puteri, Johor"},
	{"Johor", "Iskandar Puteri", "Legoland Hotel Malaysia", "No 7, Jln Legoland, Bandar, 79250 Johor Bahru, Johor"},
	{"Johor", "Desaru", "Hard Rock Hotel Desaru Coast", "Desaru Coast, Jln Pantai 3, 81930 Bandar Penawar, Johor"},
	{"Johor", "Desaru", "The Westin Desaru Coast Resort", "Desaru Coast, Jln Pantai 3, 81930 Bandar Penawar, Johor"},
	{"Johor", "Muar", "Classic Hotel", "69, Jalan Ali, Pekan Muar, 84000 Muar, Johor Darul Ta'zim"},
	{"Johor", "Muar", "Muar Trader's Hotel", "No. 16, Pusat Perdagangan Muar, Jalan Peteri, Jalan Petrie, Taman Sri Tanjung, 84000 Muar, Johor Darul Ta'zim"},
	{"Johor", "Mersing", "Hotel Mersing Elite", "Jalan Dato Mohd Ali, Mersing Kechil, 86800 Mersing, Johor Darul Ta'zim"},
	{"Johor", "Mersing", "Mersing Merlin Inn", "Jalan Endau, Kampung Seri Lalang, 86800 Mersing, Johor Darul Ta'zim"},
	{"Johor", "Kluang", "Hotel Setia", "Jalan Setia Indah 1, 86000 Kluang, Johor Darul Ta'zim"},
	{"Johor", "Kluang", "Kluang Parade Hotel", "Blok D, Jalan Mata Kucing No 9AB- 14B, Kampung Masjid Lama, 86000 Kluang, Johor"},
	{"Johor", "Batu Pahat", "Pine Tree Hotel", "36, Jalan Mengkudu, Taman Batu Pahat, 83000 Batu Pahat, Johor Darul Ta'zim"},
	{"Johor", "Batu Pahat", "The Katerina Hotel", "8, Jln Zabedah, Kampung Pegawai, 83000 Batu Pahat, Johor Darul Ta'zim"},
	{"Johor", "Segamat", "Hotel VIP", "Jalan Buloh Kasap, Kampung Tengah, 85000 Segamat, Johor Darul Ta'zim"},
	{"Johor", "Segamat", "Segamat Hotel", "14, Jalan Salim, Kampung Abdullah, 85000 Segamat, Johor Darul Ta'zim"},
	{"Pahang", "Genting Highlands", "Genting SkyWorlds Hotel", "Genting Highlands, 69000 Genting Highlands, Pahang"},
	{"Pahang", "Genting Highlands", "Swiss-Garden Hotel & Residences Genting Highlands", "Windmill Upon Hills, Jalan Permai, 69000 Genting Highlands, Pahang"},
	{"Pahang", "Genting Highlands", "Wyndham Ion Majestic Hotel", "Jalan Ion Delemen, 69000 Genting Highlands, Pahang"},
	{"Pahang", "Kuantan", "AC Hotel Marriott Kuantan", "Jln Teluk Sisek, 25000 Kuantan, Pahang"},
	{"Pahang", "Kuantan", "Hyatt Regency Kuantan Resort", "Teluk Cempedak, 25050 Kuantan, Pahang"},
	{"Pahang", "Kuantan", "Swiss-Belhotel Kuantan", "1, Jalan Tanjung Lumpur, Tanjung Lumpur, 26060 Kuantan, Pahang"},
	{"Pahang", "Cameron Highlands", "Cameron Highlands Resort", "By The Golf Course, Brinchang, 39000 Tanah Rata, Pahang"},
	{"Pahang", "Cameron Highlands", "Hotel De'la Ferns", "Batu 39, Jalan Besar, Tanah Rata, 39000 Tanah Rata, Pahang"},
	{"Pahang", "Cameron Highlands", "Zenith Hotel Cameron", "Jln Majlis, Tanah Rata, 39000 Tanah Rata, Pahang"},
	{"Pahang", "Cherating", "Club Med Cherating", "29th Mile, Jalan Kuantan-Kemaman, 26080 Kuantan, Pahang"},
	{"Pahang", "Cherating", "Royale Chulan Cherating Villas & Chalets", "Jalan Kampung Cherating Lama, 26100 Balok, Pahang"},
	{"Pahang", "Taman Negara", "Mutiara Taman Negara", "27000 Kuala Tahan, Pahang"},
	{"Pahang", "Taman Negara", "Xcape Resort", "Lot 813, 27000 Kuala Tahan, Pahang"},
	{"Pahang", "Tioman Island", "ABC Chalet Tioman", "Kampung Air Batang, Pulau Tioman, 26800 Kuala Rompin, Pahang"},
	{"Pahang", "Tioman Island", "Paya Beach Spa & Dive Resort", "Lot 71 & 77, Kampong Paya, Pulau Tioman, Johor"},
	{"Pahang", "Tioman Island", "Tunamaya Beach & Spa Resort", "Lot 20 & 21, Sarang Burung, Kg. Mukut, 26800 Pulau Tioman, Pahang"},
	{"Pahang", "Fraser's Hill", "Shahzan Inn Fraser's Hill", "Jalan Lady Guillemard, 49000 Bukit Fraser, Pahang"},
	{"Pahang", "Bentong", "Bilut Hills Eco Resort", "PT59, Mukim Tras, 27600 Bentong, Pahang"},
	{"Pahang", "Bentong", "Colmar Tropicale (Bukit Tinggi)", "KM 48, Lebuhraya Karak, Bukit Tinggi, 28750 Bentong, Pahang"},
	{"Terengganu", "Kuala Terengganu", "Duyong Marina & Resort", "Pulau Duyong, 21300 Kuala Terengganu, Terengganu"},
	{"Terengganu", "Kuala Terengganu", "Primula Beach Hotel", "Jalan Persinggahan, 20400 Kuala Terengganu, Terengganu"},
	{"Terengganu", "Kuala Terengganu", "Raia Hotel & Convention Centre Terengganu", "Jalan Lapangan Terbang, Kampung Telaga Batin, 21300 Kuala Terengganu, Terengganu"},
	{"Terengganu", "Besut", "Mimpi Perhentian", "Long Beach, Pulau Perhentian Kecil, 22300 Besut, Terengganu"},
	{"Terengganu", "Besut", "Tuna Bay Island Resort", "Kampung Pulau Perhentian, 22300 Besut, Terengganu"},
	{"Terengganu", "Dungun", "Tanjong Jara Resort", "Batu 8, Off Jalan Dungun, 23000 Kuala Dungun, Terengganu"},
	{"Terengganu", "Kuala Nerus", "The Serai Cottage Transit Hotel", "Lot 29768 & 29769, Tingkat 1 Taman Bestari Jalan ILP, Kampung Gong Badak, 21300, Terengganu"},
	{"Terengganu", "Heritage Waterfront Area", "Felda Residence Kuala Terengganu", "Jalan Hiliran, 20300 Kuala Terengganu, Terengganu"},
	{"Terengganu", "Heritage Waterfront Area", "Ming Paragon Hotel", "219D, Jalan Sultan Zainal Abidin, 20200 Kuala Terengganu, Terengganu"},
	{"Terengganu", "Hulu Terengganu", "Kenyir Eco Resort", "Jalan Pengkalan Utama, Tasik Kenyir, 21700 Kuala Berang, Terengganu"},
	{"Terengganu", "Kemaman", "Resort World Kijal", "KM 28, Jalan Kemaman-Dungun, Kampung Meraga Beris, 24210 Kijal, Terengganu"},
	{"Kelantan", "Kota Bharu", "Grand Renai Hotel", "Jalan Sultan Yahya Petra, Kota Sri Mutiara, 15150 Kota Bharu, Kelantan"},
	{"Kelantan", "Kota Bharu", "Perdana Kota Bharu", "Jln Mahmood, Bandar Kota Bharu, 15000 Kota Bharu, Kelantan"},
	{"Kelantan", "Bachok", "Andiana Beach & Resort", "Lot 1105, Kampung Pulau Belongan, 16300 Bachok, Kelantan"},
	{"Kelantan", "Bachok", "Villa Beau Rivage Bachok", "PT 22523, Kg Aur, 16300 Bachok, Kelantan"},
	{"Kelantan", "Pasir Puteh", "Aleesa Roomstay", "Lot 912, Kampung Gelam, 16800 Pasir Puteh, Kelantan"},
	{"Kelantan", "Pasir Puteh", "Tok Aman Bali Beach Resort", "6264, 1, Pantai Bisikan Bayu, 16700 Cherang Ruku, Kelantan"},
	{"Kelantan", "Gua Musang", "Mines Inn Hotel", "PT 8214, Jalan Persiaran Raya, 18300 Gua Musang, Kelantan"},
	{"Kelantan", "Gua Musang", "Moonriver Lodge", "Jalan Gua Musang - Cameron Highlands, 18300 Lojing, Kelantan"},
	{"Kelantan", "Tanah Merah", "Humaira Hotel", "PT 833, Kompleks Perniagaan Humaira, Jalan Humaira 1, Tanah Merah, 17500 Tanah Merah, Kelantan"},
	{"Kelantan", "Jeli", "Jeli Inn", "PT 5623, Jeli inn, Lebuhraya Timur-Barat, Kg Gemang Baru, 17700 Ayer Lanas, Kelantan"},
	{"Kelantan", "Pasir Mas", "Mas Inn Hotel", "Lot 5200, Jalan Pasir Mas - Rantau Panjang, 17070 Pasir Mas, Kelantan"},
	{"Sabah", "Kota Kinabalu", "The Magellan Sutera Resort", "1 Sutera Harbour Boulevard, Sutera Harbour, 88100 Kota Kinabalu, Sabah"},
	{"Sabah", "Kota Kinabalu", "The Pacific Sutera Hotel", "1 Sutera Harbour Boulevard, Sutera Harbour, 88100 Kota Kinabalu, Sabah"},
	{"Sabah", "Sandakan", "Four Points by Sheraton Sandakan", "Harbour Square, Pusat Bandar Sandakan, 90000 Sandakan, Sabah"},
	{"Sabah", "Sandakan", "Sepilok Nature Resort", "1.5KM, Jalan Sepilok, Sepilok, 90000 Sandakan, Sabah"},
	{"Sabah", "Tawau", "Borneo Royale Hotel", "Mile 1, Jalan Kuhara, 91000 Tawau, Sabah"},
	{"Sabah", "Tawau", "L A Hotel", "38, Jalan Haji Karim, Bandar Tawau, 91000 Tawau, Sabah"},
	{"Sabah", "Semporna", "Sipadan Water Village Resort", "Mabul Island, 91031 Mabul, Sabah"},
	{"Sabah", "Kundasang", "Pine Resort", "Pekan Kundasang, 89308 Ranau, Sabah"},
	{"Sabah", "Kundasang", "Sutera Sanctuary Lodges at Kinabalu Park", "Kinabalu National Park, Kundasang, 89300 Ranau, Sabah"},
	{"Sabah", "Lahad Datu", "Borneo Rainforest Lodge", "Lot 20, Block 3, MDLD 3285, Ground Floor, Fajar Centre Fajar Lorong 9, 91120 Lahad Datu, Sabah"},
	{"Sabah", "Lahad Datu", "My Inn Hotel Lahad Datu", "5th-8th Floor, Lot264, Lorong Bandar Perdana 6, Sarip, 91100 Lahad Datu, Sabah"},
	{"Sabah", "Keningau", "Hotel Perkasa Keningau", "129, KM 0.2, Jalan Kampung, 89007 Keningau, Sabah"},
	{"Sabah", "Keningau", "Manis Manis Rooftop Resort", "89000 Keningau, Sabah"},
	{"Sabah", "Kudat", "Kotak Kotak Cabin", "Jln Tanjung Simpang Mengayau, 89050 Kudat, Sabah"},
	{"Sabah", "Kudat", "Kudat Riviera", "Jln Pantai Kelambu, Kampung Bawang Jamal, 89050 Kudat, Sabah"},
	{"Sarawak", "Kuching", "Sheraton Kuching Hotel", "2, Jln Padungan, 93100 Kuching, Sarawak"},
	{"Sarawak", "Kuching", "The Waterfront Hotel", "68, Jalan Tun Abang Haji Openg, 93000 Kuching, Sarawak"},
	{"Sarawak", "Miri", "Merdeka Suite Hotel (Anfasar)", "Bintulu-Miri Hwy, 98000 Miri, Sarawak"},
	{"Sarawak", "Miri", "Miri Marriott Resort & Spa", "Jalan Temenggong Datuk Oyong Lawai, 98000 Miri, Sarawak"},
	{"Sarawak", "Sibu", "Kingwood Hotel Sibu", "No.12, Lorong Lanang 4, Pekan Sibu, 96000 Sibu, Sarawak"},
	{"Sarawak", "Sibu", "RH Hotel Sibu", "Jln Kampung Nyabor, Pekan Sibu, 96000 Sibu, Sarawak"},
	{"Sarawak", "Bintulu", "Imperial Hotel Bintulu", "Lot 483 Block, 31, Jalan Tanjung Batu, Kemena Land District, 97000 Bintulu, Sarawak"},
	{"Sarawak", "Bintulu", "Parkcity Everly Hotel Bintulu", "Jalan Tun Razak, 97000 Bintulu, Sarawak"},
	{"Sarawak", "Mulu", "Mulu Marriott Resort & Spa", "Sungai Melinau, 98008 Mulu, Sarawak"},
	{"Sarawak", "Kapit", "Hotel Orchard Kapit", "Kapit, Sarawak"},
	{"Sarawak", "Kapit", "Meligai Hotel Kapit", "96800, Lot 334, Jalan Airport, 96800 Kapit, Sarawak"},
	{"Sarawak", "Sematan", "Sematan Palm Beach Resort", "Lot 295, Kpg Sungai Kilong, Pekan Sematan, 94100 Sematan, Sarawak"},
};

vector<HotelBranch> hotelBranches = [] {
	vector<HotelBranch> v;
	const size_t n = sizeof(HOTEL_ROWS) / sizeof(HOTEL_ROWS[0]);
	v.reserve(n);
	for (size_t i = 0; i < n; ++i) {
		v.push_back({HOTEL_ROWS[i][0], HOTEL_ROWS[i][1], HOTEL_ROWS[i][2], HOTEL_ROWS[i][3]});
	}
	return v;
}();


string currentHotelName = "";
string currentHotelAddress = "";
string currentHotelArea = "";
string currentHotelState = "";

vector<AddOnItem> addOnList = {
	{"Breakfast Buffet", 35.00, "per person / night"},
	{"Airport Pickup", 80.00, "per stay"},
	{"Extra Bed", 50.00, "per night"},
	{"Spa Relaxation", 120.00, "per person"},
	{"Late Checkout (2pm)", 40.00, "per stay"},
	{"Mini Bar Package", 60.00, "per stay"},
	{"Romantic Room Setup", 150.00, "per stay"},
	{"Birthday Cake", 80.00, "per stay"},
	{"KL City Tour Ticket", 90.00, "per person"}
};

bool customerExists(const string& username) {
	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].username == username) {
			return true;
		}
	}
	return false;
}

void loadUnpaidIntoSession() {
	currentSessionIDs.clear();
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername == currentLoggedInCustomer
			&& reservations[i].paymentStatus == "Unpaid"
			&& reservations[i].status != "Cancelled") {
			currentSessionIDs.push_back(reservations[i].reservationID);
		}
	}
}

void resetSessionExtras() {
	sessionPromoCode = "";
	sessionPromoPercent = 0.0;
	sessionPromoFlat = 0.0;
	sessionRedeemedPoints = 0;
	sessionSurpriseGiven = false;
}

void clearCurrentHotel() {
	currentHotelName = "";
	currentHotelAddress = "";
	currentHotelArea = "";
	currentHotelState = "";
}

void resetOccupiedRooms() {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status == "Occupied") {
			roomList[i].status = "Available";
		}
	}
}

void setRoomStatus(const string& roomNumber, const string& status) {
	int idx = findRoomIndex(roomNumber);
	if (idx != -1) {
		roomList[idx].status = status;
	}
}

void removeFromCurrentSession(const string& reservationID) {
	vector<string> kept;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		if (currentSessionIDs[i] != reservationID) {
			kept.push_back(currentSessionIDs[i]);
		}
	}
	currentSessionIDs = kept;
	if (currentSessionIDs.empty()) {
		resetSessionExtras();
	}
}

void syncRoomOccupancy() {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status == "Cleaning" || roomList[i].status == "Maintenance") {
			continue;
		}

		bool used = false;
		for (size_t r = 0; r < reservations.size(); r++) {
			if (reservations[r].status == "Cancelled") {
				continue;
			}
			if (reservations[r].roomNumber == roomList[i].roomNumber) {
				used = true;
				break;
			}
		}
		roomList[i].status = used ? "Occupied" : "Available";
	}
}

string generateReservationID() {
	return padNumber(nextReservationID, 4);
}

int findRoomIndex(const string& roomNumber) {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].roomNumber == roomNumber) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

int findCustomerIndex(const string& username) {
	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].username == username) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

int findReservationIndex(const string& reservationID) {
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].reservationID == reservationID) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

string membershipOfCurrentUser() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return "Regular";
	}
	return customers[idx].membershipStatus;
}

int loyaltyPointsOfCurrentUser() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return 0;
	}
	return customers[idx].loyaltyPoints;
}

void addLoyaltyPoints(int points) {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return;
	}
	customers[idx].loyaltyPoints += points;
	refreshMembershipTier(idx);
}

void refreshMembershipTier(int customerIndex) {
	if (customers[customerIndex].loyaltyPoints >= GOLD_POINTS) {
		customers[customerIndex].membershipStatus = "Gold";
	}
	else if (customers[customerIndex].loyaltyPoints >= SILVER_POINTS) {
		customers[customerIndex].membershipStatus = "Silver";
	}
	else {
		customers[customerIndex].membershipStatus = "Regular";
	}
}

double membershipRate(const string& status) {
	if (status == "Gold") {
		return 0.10;
	}
	if (status == "Silver") {
		return 0.05;
	}
	return 0.0;
}

string currentCustomerName() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return currentLoggedInCustomer;
	}
	if (customers[idx].fullName.empty() || customers[idx].fullName == "---") {
		return customers[idx].username;
	}
	return customers[idx].fullName;
}
