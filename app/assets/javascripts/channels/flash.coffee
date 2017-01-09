App.flash = App.cable.subscriptions.create "FlashChannel",
  connected: ->
    # Called when the subscription is ready for use on the server

  disconnected: ->
    # Called when the subscription has been terminated by the server

  received: (data) ->
    $("#err").text(data.message)
