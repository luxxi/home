App.lights = App.cable.subscriptions.create "LightsChannel",
  connected: ->
    @perform 'test'

  disconnected: ->
    # Called when the subscription has been terminated by the server

  received: (data) ->
    id = data.variables[1].id
    value = data.variables[1].value
    $("#"+id).prop('checked', value);
