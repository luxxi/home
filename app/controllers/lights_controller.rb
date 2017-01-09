class LightsController < ApplicationController
  # before_action :connect

  def index
    # ActionCable.server.broadcast 'lights_channel', @device.variables
    # LightsStatusJob.perform_later("test")
  end

  private

  # def connect
  #   @device = Particle.device(ENV['AMBIENT_ID'])
  #   flash[:error] = "Device not connected!" unless @device.connected?
  # end
end
