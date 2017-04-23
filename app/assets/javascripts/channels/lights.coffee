App.lights = App.cable.subscriptions.create "LightsChannel",
  connected: ->
    $("#closet").change =>
      @perform 'test', light: "closet", state: this.is(':checked')
    $("#picture").change =>
      @perform 'test', light: "picture", state: this.is(':checked')
    $("#ambient").change =>
      @perform 'test', light: "ambient", state: this.is(':checked')

  received: (data) ->
    id = data.variables[1].id
    value = data.variables[1].value
    $("#"+id).prop('checked', value);
