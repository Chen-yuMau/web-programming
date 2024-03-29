from peewee import *


db = SqliteDatabase('database.db', pragmas={'foreign_keys': 1})


class BaseModel(Model):
    class Meta:
        database = db


class User(BaseModel):
    username = CharField(unique=True)
    password = CharField()


class Invitation(BaseModel):
    inviter = ForeignKeyField(User, on_delete='CASCADE')
    invitee = ForeignKeyField(User, on_delete='CASCADE')


class Friend(BaseModel):
    user = ForeignKeyField(User, on_delete='CASCADE')
    friend = ForeignKeyField(User, on_delete='CASCADE')


class Post(BaseModel):
    user = ForeignKeyField(User, on_delete='CASCADE')
    message = CharField()


class Follow(BaseModel):
    follower = ForeignKeyField(User, on_delete='CASCADE')
    followee = ForeignKeyField(User, on_delete='CASCADE')


class Token(BaseModel):
    token = CharField(unique=True)
    owner = ForeignKeyField(User, on_delete='CASCADE')

class Groupname(BaseModel):
    name = CharField(unique=True)

class Group(BaseModel):
    name = ForeignKeyField(Groupname, on_delete='CASCADE')
    user = ForeignKeyField(User, on_delete='CASCADE')


if __name__ == '__main__':
    db.connect()
    db.create_tables([User, Invitation, Friend, Post, Follow, Token, Groupname, Group])
