export class User {
    constructor(id, name, dateOfBirth) {
        this.id = id;
        this.name = name;
        this.dateOfBirth = dateOfBirth;
    }
}
export class DB {
    static async select(array) {
        return array.sort((a,b)=>a.id-b.id);
    }
    static async insert(row) {
        let lastId = Math.max(...database.map(item => item.id), 0) + 1;
        const id = lastId;
        const newRow = { id, ...row };
        database.push(newRow);
        return newRow;
    }
    static async update(newData) {
        const index = database.findIndex(item => item.id === Number.parseInt(newData.id));
        if (index !== -1) {
            database.splice(index, 1);
            database.push(newData)
            return database[database.length-1];
        }
        return null;
    }
    static async delete(idD) {
        const index = database.findIndex(item => item.id === Number.parseInt(idD));
        if (index !== -1) {
            return database.splice(index, 1)[0];
        }
        return null;
    }
    static async commit() {
        console.log("Database state committed.");
        return { message: "Database state committed." };
    }
}
export let database = [
    new User(1, "Anya", "2015-11-09"),
    new User(2, "Sasha", "2013-03-03"),
    new User(3, "Grisha", "2008-12-11"),
    new User(4, "Sonya", "2007-11-01"),
    new User(5, "Slava", "2016-03-08")
];