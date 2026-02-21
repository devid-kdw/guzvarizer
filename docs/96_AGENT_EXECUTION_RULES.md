# Agent Execution Rules

## Workspace-only Enforcement

Svi agenti moraju:

- postaviti `cwd` na `/Users/grzzi/Desktop/Kompresor`
- koristiti relativne putanje unutar repozitorija
- ne koristiti komandne putanje koje ciljaju direktorije izvan `Kompresor`

## Allowed install behavior

Dozvoljeno:

- instalacija build/development dependency-ja potrebnih za trenutnu fazu
- kreiranje lokalnih dependency cache foldera unutar projekta (npr. `.deps/`, `build/`)

Nije dozvoljeno bez odobrenja:

- global system-level promjene
- izmjene shell/profile konfiguracija korisnika

## Phase stop policy

- Agent ne smije samostalno krenuti u iducu fazu bez phase reporta.
- Svaka faza zavrsava tek kad je report predan.
