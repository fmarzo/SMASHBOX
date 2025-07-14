import asyncio
from functools import partial
from turtledemo.chaos import jumpto

from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes
from config import TOKEN_TG_BOT, CHAT_ID_TG_BOT
from database.firebase_db import FirebaseDB


async def cmd_start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Response to /start"""
    chat_id = update.message.chat_id
    await update.message.reply_text(chat_id)

async def cmd_ask_log(update: Update, context: ContextTypes.DEFAULT_TYPE, box, client) -> None:
    """Response to /log"""
    firebase_db = FirebaseDB()
    loop = asyncio.get_running_loop()
    logs = await loop.run_in_executor(None, firebase_db.get_logs, box, client)

    if logs:
        formatted_logs = "\n".join([f"Data: {log.split()[0]}, Ora: {log.split()[1]}" for log in logs])
    else:
        formatted_logs = "No logs found!."

    try:
        await update.message.reply_text(f"{formatted_logs}")
    except Exception as e:
        try:
            await update.message.reply_text(f"{formatted_logs}")
        except Exception as retry_error:
            print(f"Errore nel tentativo di invio del messaggio: {retry_error}")


class TgBot:
    _instance = None
    _initialized = False
    def __new__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self, box, client):
        """Init bot"""
        if not self._initialized:
            TgBot._initialized = True

            self.__app = ApplicationBuilder().token(TOKEN_TG_BOT).build()
            self.__app.add_handler(CommandHandler("start", cmd_start))
            self.__app.add_handler(CommandHandler("log", partial(cmd_ask_log, box= box, client= client)))
            self.loop = None

    @staticmethod
    def get_instance():
        if TgBot._instance is None:
            raise ValueError("TgBot non è stato inizializzato. Assicurati di chiamare TgBot() prima di usarlo.")
        return TgBot._instance

    def run_bot(self):
        """Start bot in a separate thread"""
        self.loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self.loop)

        try:
            self.loop.run_until_complete(self.__app.run_polling())
        except asyncio.CancelledError:
            print("Bot stopped")
        finally:
            self.loop.run_forever()

    async def send_msg(self, chat_id: int, message: str):
        """Invia un messaggio a un chat ID specifico."""
        if not self.__app.bot:
            print("Errore: il bot non è inizializzato correttamente.")
            return

        try:
            await self.__app.bot.send_message(chat_id=chat_id, text=message)
            print(f"Messaggio inviato a {chat_id}: {message}")
        except Exception as e:
            print(f"Errore nell'invio del messaggio: {e}")


#TODO
# MAKE A NEW BOT OFFICIAL
# FIND OUT HOW TO RETRIEVE DATE FROM THINGSBOARD (USING MQTT? OTHERWISE, SAVE DATE INTO FIREBASE E RETRIEVE IT FROM THERE?)