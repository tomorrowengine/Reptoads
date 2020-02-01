#ifndef CONTRIBUTORS_INCLUDED
#define CONTRIBUTORS_INCLUDED





struct show_table_contributors_st {
  const char *name;
  const char *location;
  const char *comment;
};



struct show_table_contributors_st show_table_contributors[]= {
  
  {"Booking.com", "https://www.booking.com", "Founding member, Platinum Sponsor of the MariaDB Foundation"},
  {"Alibaba Cloud", "https://www.alibabacloud.com/", "Platinum Sponsor of the MariaDB Foundation"},
  {"Tencent Cloud", "https://cloud.tencent.com", "Platinum Sponsor of the MariaDB Foundation"},
  {"Microsoft", "https://microsoft.com/", "Platinum Sponsor of the MariaDB Foundation"},
  {"MariaDB Corporation", "https://mariadb.com", "Founding member, Platinum Sponsor of the MariaDB Foundation"},
  {"Visma", "https://visma.com", "Gold Sponsor of the MariaDB Foundation"},
  {"DBS", "https://dbs.com", "Gold Sponsor of the MariaDB Foundation"},
  {"IBM", "https://www.ibm.com", "Gold Sponsor of the MariaDB Foundation"},
  {"Tencent Games", "http://game.qq.com/", "Gold Sponsor of the MariaDB Foundation"},
  {"Nexedi", "https://www.nexedi.com", "Silver Sponsor of the MariaDB Foundation"},
  {"Acronis", "https://www.acronis.com", "Silver Sponsor of the MariaDB Foundation"},
  {"Verkkokauppa.com", "https://www.verkkokauppa.com", "Bronze Sponsor of the MariaDB Foundation"},
  {"Virtuozzo", "https://virtuozzo.com", "Bronze Sponsor of the MariaDB Foundation"},
  {"Tencent Game DBA", "http://tencentdba.com/about", "Bronze Sponsor of the MariaDB Foundation"},
  {"Tencent TDSQL", "http://tdsql.org", "Bronze Sponsor of the MariaDB Foundation"},
  {"Percona", "https://www.percona.com/", "Bronze Sponsor of the MariaDB Foundation"},

  
  {"Google", "USA", "Sponsoring encryption, parallel replication and GTID"},
  {"Facebook", "USA", "Sponsoring non-blocking API, LIMIT ROWS EXAMINED etc"},

  
  {"Ronald Bradford", "Brisbane, Australia", "EFF contribution for UC2006 Auction"},
  {"Sheeri Kritzer", "Boston, Mass. USA", "EFF contribution for UC2006 Auction"},
  {"Mark Shuttleworth", "London, UK.", "EFF contribution for UC2006 Auction"},
  {NULL, NULL, NULL}
};

#endif 
