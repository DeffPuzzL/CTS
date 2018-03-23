alter table tbl_r_lmer_con add match_flag varchar2(1) default '0' not null;
comment on column tbl_r_lmer_con.match_flag is '模糊匹配开关';
alter table tbl_r_cha_con add match_flag varchar2(1) default '0' not null;
comment on column tbl_r_cha_con.match_flag is '模糊匹配开关';
alter table tbl_r_cmer_con add match_flag varchar2(1) default '0' not null;
comment on column tbl_r_cmer_con.match_flag is '模糊匹配开关';
