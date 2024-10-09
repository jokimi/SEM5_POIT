
-- 1 (5)

select * from dba_pdbs;

-- 2

select * from V$INSTANCE;

-- 3

SELECT comp_id, comp_name, version, status FROM dba_registry;

-- 4

ALTER PLUGGABLE DATABASE KEM_PDB OPEN;
ALTER PLUGGABLE DATABASE KEM_PDB CLOSE;