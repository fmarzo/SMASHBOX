import asyncio
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, ContextTypes
from config import TOKEN_TG_BOT

async def cmd_start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Response to /start"""
    await update.message.reply_text("Estamos a tope Tucu")

async def cmd_ask_log(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Response to /log"""
    await update.message.reply_text("Richiesta di log")

class TgBot:
    def __init__(self):
        """Init bot"""
        self.__app = ApplicationBuilder().token(TOKEN_TG_BOT).build()
        self.__app.add_handler(CommandHandler("start", cmd_start))
        self.__app.add_handler(CommandHandler("log", cmd_ask_log))

        self.loop = None

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
