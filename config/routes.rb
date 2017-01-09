Rails.application.routes.draw do
  root to: 'lights#index'
  mount ActionCable.server => '/cable'
end
